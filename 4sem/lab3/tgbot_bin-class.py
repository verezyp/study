import telebot
import os
import hashlib
import sqlite3
import numpy as np
import tensorflow as tf
from tensorflow.keras.preprocessing import image
import io
import warnings

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
# Settings the warnings to be ignored
warnings.filterwarnings('ignore')

print("Bot started...")
bot = telebot.TeleBot('token')
BASE_DIR = os.path.abspath("database")
print(BASE_DIR)
conn = sqlite3.connect(BASE_DIR, check_same_thread=False)
cursor = conn.cursor()


def reg_new_user(message):
    password = message.text
    if len(password) >= 3 or ('/' in password):
        bot.send_message(message.from_user.id, 'Repeat your password')
        bot.register_next_step_handler(message, repeated_passwd, password)
    else:
        bot.send_message(message.from_user.id, 'Bad password, retry /register')
        bot.register_next_step_handler(message, get_text_messages)
    return


def repeated_passwd(message, password):
    if message.text == password:
        in_db(message.chat.id, password)
        bot.send_message(message.from_user.id, 'Registration successful')
    else:
        bot.send_message(message.from_user.id, 'Wrong password, retry /register')
        bot.register_next_step_handler(message, get_text_messages)
    return


@bot.message_handler(content_types=['text', 'photo'])
def get_text_messages(message):
    # print(message.chat.id)
    match message.text:

        case "/start":
            bot.send_message(message.from_user.id, 'Hi! I\'m binary-classifier image bot.'
                                                   ' Type /help to see the command list')

        case "/help":
            bot.send_message(message.from_user.id,
                             "/register – команда для регистрации пользователя\n"
                             "/login – команда для прохождения аутентификации\n"
                             "/predict – [только при предшествующей аутентификации] "
                             "команда для проведения бинарной классификации картинки;"
                             " после отправки данной команды пользователю необходимо отправить боту картинку;"
                             " в ответ бот присылает ответ – на картинке изображен человек или горилла\n"
                             "/logout – команда для выхода из системы")

        case "/register":
            cursor.execute("SELECT * FROM test WHERE chat_id = ?", (message.chat.id,))
            results = cursor.fetchall()
            if results:
                bot.send_message(message.from_user.id, 'You are already registered, if you forgot the password,'
                                                       ' contact @verezyp')
                bot.register_next_step_handler(message, get_text_messages)
            else:
                bot.send_message(message.from_user.id, "Insert password (len >= 3)")
                bot.register_next_step_handler(message, reg_new_user)

        case "/login":
            cursor.execute("SELECT * FROM test WHERE chat_id = ?", (message.chat.id,))
            result = cursor.fetchone()
            if result is None:
                bot.send_message(message.from_user.id, "You cannot use this command until you are register in")
            else:
                if result[3] == 1:
                    bot.send_message(message.from_user.id, "You are already logged in")
                    bot.register_next_step_handler(message, get_text_messages)
                else:
                    salted_key_db = result[2]
                    bot.send_message(message.from_user.id, "Enter a password")
                    bot.register_next_step_handler(message, login, salted_key_db)

        case "/logout":
            cursor.execute("SELECT * FROM test WHERE chat_id = ?", (message.chat.id,))
            result = cursor.fetchone()
            if result is None:
                bot.send_message(message.from_user.id, "You cannot use this command until you are register in")
            else:
                if result[3] == 0:
                    bot.send_message(message.from_user.id, "You are already logged out")
                    bot.register_next_step_handler(message, get_text_messages)
                else:
                    cursor.execute('UPDATE test SET current_authentication=? WHERE chat_id=?', (0, message.chat.id))
                    conn.commit()
                    bot.send_message(message.from_user.id, "Logout successfully")

        case "/predict":
            cursor.execute("SELECT * FROM test WHERE chat_id = ?", (message.chat.id,))
            result = cursor.fetchone()
            if result is None:
                bot.send_message(message.from_user.id, "You cannot use this command until you are register in")
            else:
                if result[3] == 1:
                    bot.send_message(message.from_user.id, "Send photo, which you want to analyse")
                    bot.register_next_step_handler(message, get_photo)
                else:
                    bot.send_message(message.from_user.id, "You cannot use this command until you are logged in")
        case _:
            bot.send_message(message.from_user.id, "Incorrect command. Try /help")
    return  # ??????


def in_db(chat_id: int, raw_password: str):
    salt = os.urandom(32)
    key = hashlib.pbkdf2_hmac('sha256', raw_password.encode('utf-8'), salt, 100000)
    salted_key = salt + key  # to db-> chat_id:salted_key:cur_auth(#(0/1) now 0)
    cur_auth = 0
    cursor.execute('INSERT INTO test (chat_id, salted_key, current_authentication) VALUES (?, ?, ?)',
                   (chat_id, salted_key, cur_auth))
    conn.commit()
    return


def login(message, salted_key_db):
    in_passwd = message.text
    salt_from_storage = salted_key_db[:32]
    key_from_storage = salted_key_db[32:]
    try_key = hashlib.pbkdf2_hmac('sha256', in_passwd.encode('utf-8'), salt_from_storage, 100000)
    if key_from_storage == try_key:
        cursor.execute('UPDATE test SET current_authentication=? WHERE chat_id=?', (1, message.chat.id))
        conn.commit()
        bot.send_message(message.from_user.id, "Login successful")
    else:
        bot.send_message(message.from_user.id, "Wrong password")
        bot.register_next_step_handler(message, get_text_messages)
    return


def get_photo(message):
    if message.text:
        bot.send_message(message.from_user.id, "Incorrect input")
        bot.register_next_step_handler(message, get_text_messages)
    else:
        if message.photo:
            file_ID = message.photo[-1].file_id
            file_info = bot.get_file(file_ID)
            downloaded_file = bot.download_file(file_info.file_path)
            if file_info.file_path.endswith(".jpg"):
                with open("image.jpg", 'wb') as new_file:
                    new_file.write(downloaded_file)
                with open("image.jpg", "rb") as new_file:
                    img_bytes = io.BytesIO(new_file.read())
                    model = tf.keras.models.load_model('model.h5')
                    img = image.load_img(img_bytes, target_size=(200, 200))  # validvar/gorillas/{filename}
                    x = image.img_to_array(img)
                    x = np.expand_dims(x, axis=0)
                    images = np.vstack([x])
                    classes = model.predict(images, batch_size=1)
                    if classes[0] < 0.5:
                        bot.send_message(message.from_user.id, "Human")
                    else:
                        bot.send_message(message.from_user.id, "Gorilla")
        else:
            bot.send_message(message.from_user.id, "Incorrect input")
    return


bot.polling(none_stop=True, interval=0)

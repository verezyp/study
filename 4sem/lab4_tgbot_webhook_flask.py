from flask import Flask, request
import requests
import json
import asyncio
import aiohttp
import sqlite3
import os
import base64
import hashlib
import random

conn = sqlite3.connect('/home/username/mysite/lab4db', check_same_thread=False)
cursor = conn.cursor()

token = '***'
endp = 'endpoint'
url = "https://username.pythonanywhere.com/" + endp
response = requests.post(
        f"https://api.telegram.org/bot{token}/setWebhook",
        json={"url": url}
)

app = Flask(__name__)


def my_send_message(chat_id, message):
    requests.get('https://api.telegram.org/bot'+token+'/sendMessage?&chat_id='+str(chat_id)+'&text='+str(message))


def prediction():
    return random.choice(['Human', 'Gorilla'])


def command_manager(command, chat_id):

    match command:
        case "/start":
            my_send_message(chat_id, 'Hi! I\'m binary-classifier image bot.'
                                                   ' Type /help to see the command list')


        case "/help":
            my_send_message(chat_id,
                             "/register – команда для регистрации пользователя\n"
                             "/login – команда для прохождения аутентификации\n"
                             "/predict – [только при предшествующей аутентификации] "
                             "команда для проведения бинарной классификации картинки;"
                             " после отправки данной команды пользователю необходимо отправить боту картинку;"
                             " в ответ бот присылает ответ – на картинке изображен человек или горилла\n"
                             "/logout – команда для выхода из системы")

        case "/register":
            cursor.execute("SELECT * FROM lab4table WHERE chat_id = ?", (chat_id,))
            results = cursor.fetchone()
            if results:

                if results[4] == 'registered':
                    my_send_message(chat_id, 'You are already registered, if you forgot the password,'
                                                           ' contact @verezyp')

                elif results[4] == 'noreg':
                    cursor.execute('UPDATE lab4table SET register_state=? WHERE chat_id=?', ('preregister', chat_id))
                    conn.commit()
                    my_send_message(chat_id, 'Enter password (len >= 3)')

                else:
                    cursor.execute('UPDATE lab4table SET register_state=? WHERE chat_id=?', ('preregister', chat_id))
                    conn.commit()
                    cursor.execute('UPDATE lab4table SET login_state=? WHERE chat_id=?', ('nologinyet' , chat_id))
                    conn.commit()
                    my_send_message(chat_id, 'Wrong password')

            else:
                my_send_message(chat_id, "Enter password (len >= 3)")
                cursor.execute('INSERT INTO lab4table (chat_id, salted_key, current_authentication, register_state, login_state) VALUES (?, ?, ?, ?, ?)',
                   (chat_id, 0, 2, 'preregister', 'nologinyet'))
                conn.commit()

        case "/login":

            cursor.execute("SELECT * FROM lab4table WHERE chat_id = ?", (chat_id,))
            result = cursor.fetchone()

            if result is None:
                my_send_message(chat_id, "You cannot use this command until you are register in")
            else:
                if result[3] == 1:
                    my_send_message(chat_id, "You are already logged in")
                else:
                    my_send_message(chat_id, "Enter a password")
                    cursor.execute('UPDATE lab4table SET login_state=? WHERE chat_id=?', ('loggining' , chat_id))
                    conn.commit()

        case "/logout":
            cursor.execute("SELECT * FROM lab4table WHERE chat_id = ?", (chat_id,))
            result = cursor.fetchone()
            if result is None:
                my_send_message(chat_id, "You cannot use this command until you are register in")
            elif result[4] != 'registered':
                my_send_message(chat_id, "You cannot use this command until you are register in")
            else:
                if result[3] != 1:
                    my_send_message(chat_id, "You are already logged out")
                else:
                    my_send_message(chat_id, "Logout...")
                    cursor.execute('UPDATE lab4table SET current_authentication=? WHERE chat_id=?', (0 , chat_id))
                    conn.commit()

        case "/predict":
            cursor.execute("SELECT * FROM lab4table WHERE chat_id = ?", (chat_id,))
            result = cursor.fetchone()
            if result is None:
                my_send_message(chat_id, "You cannot use this command until you are register in")
            elif result[4] != 'registered':
                my_send_message(chat_id, "You cannot use this command until you are register in")
            elif result[3] != 1:
                my_send_message(chat_id, "You cannot use this command until you are log in")
            else:
                my_send_message(chat_id, "Send a photo")
                cursor.execute('UPDATE lab4table SET predict_state=? WHERE chat_id=?', ('wait_photo' , chat_id))
                conn.commit()
        case _:
            my_send_message(chat_id, "Incorrect command. Try /help")
    return 0



@app.route('/' + endp, methods=['POST'])
async def webhook():

    data = request.json

    json_data = json.dumps(data)

    chat_id=data['message']['chat']['id']

    with open('logger.txt', 'a') as f:
        f.write(json_data  + '\n')

    msg = 'TESTMESSAGE'

    '''
    if 'entities' in data['message']:

        for entity in data['message']['entities']:
            if entity.get('type') == 'bot_command':
                msg = 'BOT_COMMAND'
                command_manager(data['message']['text'], chat_id)
                break '''

    if False:
        pass

    else:

        cursor.execute("SELECT * FROM lab4table WHERE chat_id = ?", (chat_id,))
        results = cursor.fetchone()

        if results:

            if results[4] == 'preregister':

                if len(data['message']['text']) >= 3:
                    my_send_message(chat_id, 'Repeat a password')

                    cursor.execute('UPDATE lab4table SET login_state=? WHERE chat_id=?', (data['message']['text'], chat_id))
                    conn.commit()

                    cursor.execute('UPDATE lab4table SET register_state=? WHERE chat_id=?', ('onepassword', chat_id))
                    conn.commit()

                else:
                    cursor.execute('UPDATE lab4table SET register_state=? WHERE chat_id=?', ('noreg', chat_id))
                    conn.commit()

                    cursor.execute('UPDATE lab4table SET login_state=? WHERE chat_id=?', ('nopasswd', chat_id))
                    conn.commit()

                    my_send_message(chat_id, 'Incorrect password, try /register again')


            elif results[4] == 'onepassword':

                if data['message']['text'] == results[5]:

                    cursor.execute('UPDATE lab4table SET register_state=? WHERE chat_id=?', ('registered', chat_id))
                    conn.commit()

                    salt = os.urandom(32)
                    key = hashlib.pbkdf2_hmac('sha256', data['message']['text'].encode('utf-8'), salt, 100000)
                    salted_key = salt + key  # to db-> chat_id:salted_key:cur_auth(#(0/1) now 0)

                    cursor.execute('UPDATE lab4table SET salted_key=? WHERE chat_id=?', (salted_key, chat_id))
                    conn.commit()

                    my_send_message(chat_id, 'registr success')

                    cursor.execute('UPDATE lab4table SET login_state=? WHERE chat_id=?', ('logout', chat_id))
                    conn.commit()

                else:
                    cursor.execute('UPDATE lab4table SET register_state=? WHERE chat_id=?', ('noreg', chat_id))
                    conn.commit()

                    cursor.execute('UPDATE lab4table SET login_state=? WHERE chat_id=?', ('nopasswd', chat_id))
                    conn.commit()

                    my_send_message(chat_id, 'Wrong repeated password, try /register again')

            elif results[5] == 'loggining':

                cursor.execute("SELECT * FROM lab4table WHERE chat_id = ?", (chat_id,))
                result = cursor.fetchone()

                in_passwd = data['message']['text']

                salt_from_storage = result[2][:32]

                key_from_storage = result[2][32:]

                try_key = hashlib.pbkdf2_hmac('sha256', in_passwd.encode('utf-8'), salt_from_storage, 100000)

                if key_from_storage == try_key:
                    cursor.execute('UPDATE lab4table SET current_authentication=? WHERE chat_id=?', (1, chat_id))
                    conn.commit()

                    cursor.execute('UPDATE lab4table SET login_state=? WHERE chat_id=?', ('login', chat_id))
                    conn.commit()

                    my_send_message(chat_id, "Login successful")

                else:
                    my_send_message(chat_id, "Wrong password")
                    cursor.execute('UPDATE lab4table SET login_state=? WHERE chat_id=?', ('nologinyet', chat_id))
                    conn.commit()

            elif results[6] == 'wait_photo':
                if 'photo' in data['message']:

                    photo = data['message']['photo'][0]['file_id']
                    file_info_url = f"https://api.telegram.org/bot{token}/getFile?file_id={photo}"
                    #dt = json.loads(photo)
                    responsee = requests.get(file_info_url)

                    file_info = responsee.json()

                    file_path = file_info["result"]["file_path"]

                    photo_url = f"https://api.telegram.org/file/bot{token}/{file_path}"
                    photo_response = requests.get(photo_url)

                    with open("photo.jpg", "wb") as photo_file:
                        photo_file.write(photo_response.content)

                    #AI operating ...

                    answer = prediction()

                    my_send_message(chat_id, answer)

                    cursor.execute('UPDATE lab4table SET predict_state=? WHERE chat_id=?', ('no', chat_id))
                    conn.commit()

                else:
                    my_send_message(chat_id, "Incorrect input, try /predict again")
                    cursor.execute('UPDATE lab4table SET predict_state=? WHERE chat_id=?', ('no' , chat_id))
                    conn.commit()
            else:
                if 'entities' in data['message']:
                    for entity in data['message']['entities']:
                        if entity.get('type') == 'bot_command':
                            msg = 'BOT_COMMAND'
                            command_manager(data['message']['text'], chat_id)
                            break
                else:
                    my_send_message(chat_id, "Incorrect input, try /help")
        else:
            if 'entities' in data['message']:
                for entity in data['message']['entities']:
                    if entity.get('type') == 'bot_command':
                        msg = 'BOT_COMMAND'
                        command_manager(data['message']['text'], chat_id)
                        break
            else:
                    my_send_message(chat_id, "Incorrect input, try /help")
    return 'OK'


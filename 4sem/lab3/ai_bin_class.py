import os
import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
from tensorflow.keras.preprocessing import image
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from sklearn.metrics import roc_curve, auc
train_dandelion_dir = os.path.join('trainvar/humans')

train_grass_dir = os.path.join('trainvar/gorillas')

valid_dandelion_dir = os.path.join('validvar/humans')

valid_grass_dir = os.path.join('validvar/gorillas')

train_dandelion_names = os.listdir(train_dandelion_dir)

train_grass_names = os.listdir(train_grass_dir)

validation_grass_names = os.listdir(valid_grass_dir)

train_datagen = ImageDataGenerator(rescale=1 / 255)
validation_datagen = ImageDataGenerator(rescale=1 / 255)

train_generator = train_datagen.flow_from_directory(
    'trainvar/',
    classes=['humans', 'gorillas'],
    target_size=(200, 200),
    batch_size=120,
    class_mode='binary')

validation_generator = validation_datagen.flow_from_directory(
    'validvar/',
    classes=['humans', 'gorillas'],
    target_size=(200, 200),
    batch_size=19,
    class_mode='binary',
    shuffle=False)

flag = 1

model = tf.keras.models.load_model('model.h5')
# if model:
#    flag = 0

if flag:
    model = tf.keras.models.Sequential([tf.keras.layers.Flatten(input_shape=(200, 200, 3)),
                                        tf.keras.layers.Dense(256, activation=tf.nn.relu),
                                        tf.keras.layers.Dense(1, activation=tf.nn.sigmoid)])

    model.summary()

    model.compile(optimizer=tf.keras.optimizers.Adam(),
                  loss='binary_crossentropy',
                  metrics=['accuracy'])

    history = model.fit(train_generator,
                        steps_per_epoch=8,
                        epochs=25,
                        verbose=1,
                        validation_data=validation_generator,
                        validation_steps=8)
    model.evaluate(validation_generator)
    STEP_SIZE_TEST = validation_generator.n // validation_generator.batch_size
    validation_generator.reset()
    preds = model.predict(validation_generator,
                          verbose=1)
    fpr, tpr, _ = roc_curve(validation_generator.classes, preds)
    roc_auc = auc(fpr, tpr)
    plt.figure()
    lw = 2
    plt.plot(fpr, tpr, color='darkorange',
             lw=lw, label='ROC curve (area = %0.2f)' % roc_auc)
    plt.plot([0, 1], [0, 1], color='navy', lw=lw, linestyle='--')
    plt.xlim([0.0, 1.0])
    plt.ylim([0.0, 1.05])
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.title('Receiver Operating Characteristic')
    plt.legend(loc="lower right")
    plt.show()
uploaded = ['test4.jpg']

if flag:
    model.save('model2.h5')

# content_dir = input("Введите расположение файла для распознавания: ")
# uploaded = [input("Введите название файла: ")]


choice = 1  # int(input("Введите количество картинок: "))
if choice == 1:
    # way = input("Напишите путь к файлу: ")
    path = "content/test4.jpg"
    cd = 0  # dandelion
    cg = 0  # grass
    # for filename in os.listdir("validvar/gorillas"):
    # f = os.path.join("content/", filename)
    img = image.load_img('testtest3.jpg',
                         target_size=(200, 200))  # validvar/gorillas/{filename}
    x = image.img_to_array(img)
    # plt.imshow(x / 255.)
    x = np.expand_dims(x, axis=0)
    images = np.vstack([x])
    classes = model.predict(images, batch_size=1)
    # print(classes[0])
    if classes[0] < 0.5:
        cd += 1
        # print("Это hum")
    else:
        cg += 1
        # print("Это gorilla")
    print(f'hum = {cd}\ngor = {cg}')


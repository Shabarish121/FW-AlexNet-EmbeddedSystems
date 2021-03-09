# -*- coding: utf-8 -*-
"""
Created on  Jun 1 14:55:18 2020

@author: Shabarish
"""


import os,cv2
import numpy as np
import matplotlib.pyplot as plt
from sklearn.utils import shuffle
from sklearn.model_selection import train_test_split
from keras.utils import np_utils
from keras.models import Sequential
from keras.layers.core import Dense, Activation, Flatten
from keras.layers.convolutional import Convolution2D, MaxPooling2D


from keras import backend as K
K.set_image_dim_ordering('th')
from keras.preprocessing.image import ImageDataGenerator
from sklearn.metrics import classification_report,confusion_matrix
import itertools

#%%

data_path = "H:\\TrainingImages"
data_dir_list = os.listdir(data_path)

img_rows=227
img_cols=227
num_epoch=1
num_channel=3
# Define the number of classes
num_classes = 4

labels_name={'Maize':0,'Roggen':1,'Triticale':2,'Wheat':3}
#labels_name={'Maize':0,'Triticale':1}

img_data_list=[]
labels_list = []

for dataset in data_dir_list: 
    img_list=os.listdir(data_path+'/'+ dataset)
    print ('Loading the images of dataset-'+'{}\n'.format(dataset))
    label = labels_name[dataset]
    for img in img_list:
	
        input_img=cv2.imread(data_path + '/'+ dataset + '/'+ img,1)
        # input_img=cv2.cvtColor(input_img, cv2.COLOR_BGR2GRAY)
#        input_img=cv2.cvtColor(input_img, cv2.COLOR_BGR2RGB)
        input_img_resize=cv2.resize(input_img,(img_rows ,img_cols),3)
        img_data_list.append(input_img_resize)
        labels_list.append(label)

img_data = np.array(img_data_list)
img_data = img_data.astype('float32')
img_data /= 255
print (img_data.shape)

labels = np.array(labels_list)
# print the count of number of samples for different classes
print(np.unique(labels,return_counts=True))
# convert class labels to on-hot encoding
Y = np_utils.to_categorical(labels, num_classes)

#Shuffle the dataset
x,y = shuffle(img_data,Y, random_state=2)
# Split the dataset
X_train, X_test, y_train, y_test = train_test_split(x, y, test_size=0.2, random_state=2)



#%%

        
# Defining the model

X_train = X_train.reshape(-1,img_rows, img_cols,3)
X_test	=X_test.reshape(-1,img_rows, img_cols,3)

model = Sequential()
model.add(Convolution2D(96,(11,11),input_shape=(img_rows,img_cols,3),data_format='channels_last',padding='valid',strides=4))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(3, 3),padding='valid',strides=2,data_format='channels_last'))


model.add(Convolution2D(256,(5,5),padding='valid',data_format='channels_last'))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(3, 3),padding='valid',strides=2,data_format='channels_last'))


model.add(Convolution2D(384,(3,3),padding='valid',data_format='channels_last'))
model.add(Activation('relu'))


model.add(Convolution2D(384,(3,3),padding='valid',data_format='channels_last'))
model.add(Activation('relu'))

model.add(Convolution2D(256,(3,3),padding='valid',data_format='channels_last'))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(3,3),padding='valid',strides=2,data_format='channels_last'))


#Passing to fully connected layers
model.add(Flatten())

#1 FC
model.add(Dense(512))#1024-512-5:512-64-5
model.add(Activation('relu'))

#2 FC
model.add(Dense(64))
model.add(Activation('relu'))


#Output layer
model.add(Dense(num_classes))
model.add(Activation('softmax'))


model.compile(loss='categorical_crossentropy', optimizer='adadelta',metrics=["accuracy"])
def saveintotxt(layer):
    with open('test.txt', 'w') as outfile:
        for i in range(0,10):
            a2 = layer.get_weights()[0][i]
            for slice_2d in a2:
                np.savetxt(outfile, slice_2d)

# Viewing model_configuration
model.summary()
model.get_config()
model.layers[0].get_config()
model.layers[0].input_shape			
model.layers[0].output_shape			
model.layers[0].get_weights()
np.shape(model.layers[0].get_weights()[0])
model.layers[0].trainable


generated_data = ImageDataGenerator(featurewise_center=False, samplewise_center=False, zoom_range=0.25,rotation_range=10,shear_range=0.2,
                                    featurewise_std_normalization=False, samplewise_std_normalization=False,zca_whitening=False,data_format='channels_last')#horizontal_flip=True)


#%%
# Training
from keras import callbacks

filename='model_train_new.csv'
csv_log=callbacks.CSVLogger(filename, separator=',', append=False)

early_stopping=callbacks.EarlyStopping(monitor='val_loss', min_delta=0, patience=0, verbose=0, mode='min')

filepath="Best-weights11-my_model-{epoch:03d}-{loss:.4f}-{acc:.4f}.hdf5"

checkpoint = callbacks.ModelCheckpoint(filepath, monitor='val_loss', verbose=1, save_best_only=True, mode='min')
callbacks_list = [csv_log,checkpoint]
callbacks_list = [csv_log,early_stopping,checkpoint]

# hist=model.fit_generator(generated_data.flow(X_train, y_train, batch_size=8), steps_per_epoch=50, epochs=num_epoch, shuffle=True,
# validation_data=(X_test, y_test),callbacks=callbacks_list)
hist = model.fit(X_train, y_train, batch_size=8, nb_epoch=num_epoch, verbose=1, validation_data=(X_test, y_test),callbacks=callbacks_list)


# visualizing losses and accuracy
train_loss=hist.history['loss']
val_loss=hist.history['val_loss']
train_acc=hist.history['acc']
val_acc=hist.history['val_acc']
xc=range(num_epoch)

plt.figure(1,figsize=(7,5))
plt.plot(xc,train_loss)
plt.plot(xc,val_loss)
plt.xlabel('num of Epochs')
plt.ylabel('loss')
plt.title('train_loss vs val_loss')
plt.grid(True)
plt.legend(['train','val'])
#print plt.style.available # use bmh, classic,ggplot for big pictures
plt.style.use(['classic'])

plt.figure(2,figsize=(7,5))
plt.plot(xc,train_acc)
plt.plot(xc,val_acc)
plt.xlabel('num of Epochs')
plt.ylabel('accuracy')
plt.title('train_acc vs val_acc')
plt.grid(True)
plt.legend(['train','val'],loc=4)
#print plt.style.available # use bmh, classic,ggplot for big pictures
plt.style.use(['classic'])

plt.figure(3,figsize=(7,5))
plt.plot(xc,train_acc)
plt.plot(xc,train_loss)
plt.xlabel('num of Epochs')
plt.ylabel('loss')
plt.title('train_acc vs train_loss')
plt.grid(True)
plt.legend(['train_acc','train_loss'])
#print plt.style.available # use bmh, classic,ggplot for big pictures
plt.style.use(['classic'])

plt.figure(4,figsize=(7,5))
plt.plot(xc,val_acc)
plt.plot(xc,val_loss)
plt.xlabel('num of Epochs')
plt.ylabel('loss')
plt.title('val_acc vs val_loss')
plt.grid(True)
plt.legend(['val_acc','val_loss'])
#print plt.style.available # use bmh, classic,ggplot for big pictures
plt.style.use(['classic'])

#%%

# Evaluating the model

score = model.evaluate(X_test, y_test, verbose=0)
print('Test Loss:', score[0])
print('Test accuracy:', score[1])



# Testing a new image
test_image = cv2.imread("H:\\Zoomed (256x256)_1\\Maize\\maize (1).jpg")
# test_image=cv2.cvtColor(test_image, cv2.COLOR_BGR2GRAY)
#test_image=cv2.cvtColor(test_image, cv2.COLOR_BGR2RGB)
test_image=cv2.resize(test_image,(img_rows,img_cols),3)
test_image = np.array(test_image)
test_image = test_image.astype('float32')
test_image /= 255
print (test_image)



# Printing the confusion matrix


Y_pred = model.predict(X_test)
print(Y_pred)
y_pred = np.argmax(Y_pred, axis=1)
print(y_pred)
target_names = ['class 0(Maize)', 'class 1(Roggen)','class 2(Triticale)','class 3(Wheat)']
#target_names = ['class 0(Maize)','class 1(Triticale)']					
print(classification_report(np.argmax(y_test,axis=1), y_pred,target_names=target_names))

print(confusion_matrix(np.argmax(y_test,axis=1), y_pred))


# Plotting the confusion matrix
def plot_confusion_matrix(cm, classes,
                          normalize=False,
                          title='Confusion matrix',
                          cmap=plt.cm.Blues):
    """
    This function prints and plots the confusion matrix.
    Normalization can be applied by setting `normalize=True`.
    """
    plt.imshow(cm, interpolation='nearest', cmap=cmap)
    plt.title(title)
    plt.colorbar()
    tick_marks = np.arange(len(classes))
    plt.xticks(tick_marks, classes, rotation=45)
    plt.yticks(tick_marks, classes)

    if normalize:
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        print("Normalized confusion matrix")
    else:
        print('Confusion matrix, without normalization')

    print(cm)

    thresh = cm.max() / 2.
    for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
        plt.text(j, i, cm[i, j],
                 horizontalalignment="center",
                 color="white" if cm[i, j] > thresh else "black")

    plt.tight_layout()
    plt.ylabel('True label')
    plt.xlabel('Predicted label')

# Compute confusion matrix
cnf_matrix = (confusion_matrix(np.argmax(y_test,axis=1), y_pred))
np.set_printoptions(precision=2)
plt.figure()

# Plot non-normalized confusion matrix
plot_confusion_matrix(cnf_matrix, classes=target_names,
                      title='Confusion matrix')
plt.show()
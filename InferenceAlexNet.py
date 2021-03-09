# -*- coding: utf-8 -*-
"""
Created on Mon Jun  8 16:42:36 2020

@author: Shabarish
"""
import numpy as np
from keras.models import Sequential
from keras.layers.convolutional import Convolution2D,MaxPooling2D
from keras.layers.core import Dense, Activation, Flatten
import cv2,os
from keras import backend as K
import pandas as pd
import xlsxwriter
from sklearn.utils import shuffle
from keras.utils import np_utils

import time

layer_num = 1
img_rows=227
img_cols=227


def buildModel():
    model = Sequential()
    model.add(Convolution2D(96,(11,11),activation='relu',input_shape=(img_rows,img_cols,3),data_format='channels_last',padding='valid',strides=4 ,name='conv1'))
    model.add(MaxPooling2D(pool_size=(3, 3),padding='valid',strides=2,data_format='channels_last'))
    
    model.add(Convolution2D(256,(5,5),activation='relu',padding='valid',name='conv2',data_format='channels_last'))
    model.add(MaxPooling2D(pool_size=(3, 3),padding='valid',strides=2,data_format='channels_last'))
    
    model.add(Convolution2D(384,(3,3),activation='relu',padding='valid',name='conv3',data_format='channels_last'))
    
    model.add(Convolution2D(384,(3,3),activation='relu',padding='valid',name='conv4',data_format='channels_last'))
    
    model.add(Convolution2D(256,(3,3),activation='relu',padding='valid',name='conv5',data_format='channels_last'))
    model.add(MaxPooling2D(pool_size=(3,3),padding='valid',strides=2,data_format='channels_last'))
    
    model.add(Flatten())
    
    model.add(Dense(512,activation='relu',name='dense1'))
    model.add(Dense(64,activation='relu',name='dense2'))
    model.add(Dense(4,name='dense3'))
    model.add(Activation('softmax'))
    return model

def loadimages():
    data_path = "H:\\Thesis\\alexnetVer3\\testImages"
       
    data_dir_list = os.listdir(data_path)
    
    
    img_rows=227
    img_cols=227
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
    	
            input_img=cv2.imread(data_path + '/'+ dataset + '/'+ img )
            # input_img=cv2.cvtColor(input_img, cv2.COLOR_BGR2GRAY)
    #        input_img=cv2.cvtColor(input_img, cv2.COLOR_BGR2RGB)
            input_img_resize=cv2.resize(input_img,(img_rows ,img_cols))
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
    # img_data = img_data.reshape(-1,3,img_rows, img_cols)
    
    return img_data,Y
def saveintotxt(layer):
    with open('conv2.txt', 'w') as outfile:
        for i in range(0,5):
            a2 = layer.get_weights()[0][i]
            for slice_2d in a2:
                np.savetxt(outfile, slice_2d)
                # saveintoxl(slice_2d,1)


def get_featuremaps(model, layer_idx, X_batch):
	get_activations = K.function([model.layers[0].input, K.learning_phase()],[model.layers[layer_idx].output,])
	activations = get_activations([X_batch,0])
	return activations

def saveintoxl(array, index):
    ## convert your array into a dataframe
    df = pd.DataFrame (array)
    ## save to xlsx file
    filepath = 'my_excel_file.xlsx'
    df.to_excel(filepath, index_label=4)

def exporttoxls(layer):
    
    nameOfFile=layer.name+'.csv'
    workbook = xlsxwriter.Workbook(nameOfFile)
    worksheet = workbook.add_worksheet()
    row = 0
    col = 0
    
    for i in range(0,layer.get_weights()[0].shape[0]):
            a2 = layer.get_weights()[0][i]
            for slice_2d in a2:
                for slice_1d in slice_2d:
                    for single_element in slice_1d:
                            worksheet.write(row, col, single_element)
                            col = col + 1
                    col = 0
                    row = row + 1

            
    workbook.close()     

def exporttoxlsone(weights):
    workbook = xlsxwriter.Workbook('batchNormalization.xlsx')
    worksheet = workbook.add_worksheet()
    row = 0
    col = 0
    for single_element in weights:
        worksheet.write(row, col, single_element)
        row = row + 1
    workbook.close()
        
def exporttoxlstwo(layer):
    
    nameOfFile=layer.name+'.csv'
    workbook = xlsxwriter.Workbook(nameOfFile)
    worksheet = workbook.add_worksheet()
    row = 0
    col = 0
    
    a2 = layer.get_weights()[0]
    for i in range(0,a2.shape[0]):        
        for j in range(0,a2.shape[1]):
            worksheet.write(row, col, a2[i][j])
            col = col + 1
        col=0
        row = row + 1

            
    workbook.close()  

def exporttocsv(layer):
    nameOfFile=layer.name+'.csv'
    with open(nameOfFile, 'w') as outfile:
        for i in range(0,layer.get_weights()[0].shape[0]):
            data = layer.get_weights()[0][i]
            for threeD_data_slice in data:
            
                for twoD_data_slice in threeD_data_slice:
                    np.savetxt(outfile, [twoD_data_slice],delimiter=',',fmt='%.9f')
def exporttocsvtwo(layer):
    nameOfFile=layer.name+'.csv'
    a2 = layer.get_weights()[0]
    with open(nameOfFile, 'w') as outfile:
        for i in range(0,a2.shape[0]):  
            data = layer.get_weights()[0][i]
            np.savetxt(outfile, [data],delimiter=',',fmt='%.9f')                    
                    
def savingfiles(model):
    for i in range(0,len(model.layers)):
        if "conv" in model.layers[i].name:
            exporttocsv(model.layers[i])
        if "dense" in model.layers[i].name:
            exporttocsvtwo(model.layers[i])

    
    


# build model and loading saved weights
model = buildModel
model.compile(loss='categorical_crossentropy', optimizer='adadelta',metrics=["accuracy"])
model.load_weights('BestWeightsTillNow2.hdf5')


# loading test images
testImages,testLabels=loadimages()

# to save weights in csv
# savingfiles(model)

# snippet to test output of individual layers
# activations = []
# for i in range(0,len(model.layers)):
#     activations.extend(get_featuremaps(model, int(i),test_image))

start_time=time.time();
print('predicted result:',model.predict(testImages))
elapsed_time=time.time()
score = model.evaluate(testImages, testLabels, verbose=0)
print('Test Loss:', score[0])
print('Test accuracy:', score[1])
print('time for inference',elapsed_time-start_time)



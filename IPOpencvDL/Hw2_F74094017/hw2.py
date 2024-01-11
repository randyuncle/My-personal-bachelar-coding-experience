# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'hw2.ui'
#
# Created by: PyQt5 UI code generator 5.15.9
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtGui import QPixmap, QPainter, QPen, QImage
from PyQt5.QtCore import Qt, QPoint
from PIL import Image
import torch
import torchvision
from torchvision import datasets
import torch.nn as nn
from torchvision import transforms
from torch.utils.data import Dataset, DataLoader
import torch
import torchsummary
import torchvision.models as models
import os
import matplotlib.pyplot as plt
import numpy as np
import cv2
import random 

classes = np.arange(10)
classes_resnet = np.array(['Cat', 'Dog'])

class CustomDataset(Dataset):
    def __init__(self, root_dir, transform=None):
        self.root_dir = root_dir
        self.transform = transform
        self.classes = sorted(os.listdir(root_dir))
        self.cat_image_path = self.choose_random_image("cat")
        self.dog_image_path = self.choose_random_image("dog")

    def choose_random_image(self, category):
        category_folder = os.path.join(self.root_dir, category)

        # Check if the specified category exists
        if not os.path.exists(category_folder):
            print(f"{category} folder not found.")
            return None

        # Get a list of all image files in the category folder
        image_files = [file for file in os.listdir(category_folder) if file.lower().endswith(('.png', '.jpg', '.jpeg'))]

        # Choose a random image from the list
        if image_files:
            random_image = random.choice(image_files)
            image_path = os.path.join(category_folder, random_image)
            return image_path
        else:
            print(f"No images found in {category} folder.")
            return None

    def __getitem__(self, index):
        # Load cat image
        cat_img = Image.open(self.cat_image_path).convert('RGB')
        cat_label = 0  # Assuming cat is class 0

        # Load dog image
        dog_img = Image.open(self.dog_image_path).convert('RGB')
        dog_label = 1  # Assuming dog is class 1

        # Apply transformations if provided
        if self.transform:
            cat_img = self.transform(cat_img)
            dog_img = self.transform(dog_img)

        return cat_img, cat_label, dog_img, dog_label

    def __len__(self):
        return 1  # Assuming we only want one pair of images
    
# Data transformation pipeline without Random Erasing
transform_resnet = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
])

class Ui_Dialog(object):
    def __init__(self):
        self.image = None
        self.resnet_image = None
        
        self.vgg19 = self.vgg19_bn()
        self.resnet50 = self.resnet50_custom()
        
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(1127, 628)
        self.globalLoadImage_pushButton = QtWidgets.QPushButton(Dialog)
        self.globalLoadImage_pushButton.setGeometry(QtCore.QRect(20, 290, 111, 41))
        self.globalLoadImage_pushButton.setObjectName("globalLoadImage_pushButton")
        self.houghCircleTransform_groupBox = QtWidgets.QGroupBox(Dialog)
        self.houghCircleTransform_groupBox.setGeometry(QtCore.QRect(160, 110, 191, 171))
        self.houghCircleTransform_groupBox.setObjectName("houghCircleTransform_groupBox")
        self.drawContour_pushButton = QtWidgets.QPushButton(self.houghCircleTransform_groupBox)
        self.drawContour_pushButton.setGeometry(QtCore.QRect(20, 30, 141, 31))
        self.drawContour_pushButton.setObjectName("drawContour_pushButton")
        self.countCoins_pushButton_2 = QtWidgets.QPushButton(self.houghCircleTransform_groupBox)
        self.countCoins_pushButton_2.setGeometry(QtCore.QRect(20, 80, 141, 31))
        self.countCoins_pushButton_2.setObjectName("countCoins_pushButton_2")
        self.houghCircleTransform_label = QtWidgets.QLabel(self.houghCircleTransform_groupBox)
        self.houghCircleTransform_label.setGeometry(QtCore.QRect(10, 130, 171, 31))
        self.houghCircleTransform_label.setObjectName("houghCircleTransform_label")
        self.histogramEqualization_groupBox_2 = QtWidgets.QGroupBox(Dialog)
        self.histogramEqualization_groupBox_2.setGeometry(QtCore.QRect(160, 310, 191, 81))
        self.histogramEqualization_groupBox_2.setObjectName("histogramEqualization_groupBox_2")
        self.histogramEqualization_pushButton_3 = QtWidgets.QPushButton(self.histogramEqualization_groupBox_2)
        self.histogramEqualization_pushButton_3.setGeometry(QtCore.QRect(20, 30, 161, 31))
        self.histogramEqualization_pushButton_3.setObjectName("histogramEqualization_pushButton_3")
        self.morphologyOperation_groupBox_3 = QtWidgets.QGroupBox(Dialog)
        self.morphologyOperation_groupBox_3.setGeometry(QtCore.QRect(160, 410, 191, 121))
        self.morphologyOperation_groupBox_3.setObjectName("morphologyOperation_groupBox_3")
        self.closing_pushButton_4 = QtWidgets.QPushButton(self.morphologyOperation_groupBox_3)
        self.closing_pushButton_4.setGeometry(QtCore.QRect(20, 20, 141, 31))
        self.closing_pushButton_4.setObjectName("closing_pushButton_4")
        self.opening_pushButton_5 = QtWidgets.QPushButton(self.morphologyOperation_groupBox_3)
        self.opening_pushButton_5.setGeometry(QtCore.QRect(20, 70, 141, 31))
        self.opening_pushButton_5.setObjectName("opening_pushButton_5")
        self.mnist_groupBox_4 = QtWidgets.QGroupBox(Dialog)
        self.mnist_groupBox_4.setGeometry(QtCore.QRect(370, 40, 691, 281))
        self.mnist_groupBox_4.setObjectName("mnist_groupBox_4")
        self.vggStructure_pushButton_6 = QtWidgets.QPushButton(self.mnist_groupBox_4)
        self.vggStructure_pushButton_6.setGeometry(QtCore.QRect(20, 40, 181, 31))
        self.vggStructure_pushButton_6.setObjectName("vggStructure_pushButton_6")
        self.mnistAccuracyLoss_pushButton_7 = QtWidgets.QPushButton(self.mnist_groupBox_4)
        self.mnistAccuracyLoss_pushButton_7.setGeometry(QtCore.QRect(20, 90, 181, 31))
        self.mnistAccuracyLoss_pushButton_7.setObjectName("mnistAccuracyLoss_pushButton_7")
        self.mnistPredict_pushButton_8 = QtWidgets.QPushButton(self.mnist_groupBox_4)
        self.mnistPredict_pushButton_8.setGeometry(QtCore.QRect(20, 140, 181, 31))
        self.mnistPredict_pushButton_8.setObjectName("mnistPredict_pushButton_8")
        self.mnistReset_pushButton_9 = QtWidgets.QPushButton(self.mnist_groupBox_4)
        self.mnistReset_pushButton_9.setGeometry(QtCore.QRect(20, 190, 181, 31))
        self.mnistReset_pushButton_9.setObjectName("mnistReset_pushButton_9")
        self.mnsitDrawBoard = QtWidgets.QLabel(self.mnist_groupBox_4)
        self.mnsitDrawBoard.setGeometry(QtCore.QRect(240, 30, 431, 221))
        self.mnsitDrawBoard.setMouseTracking(True)
        self.mnsitDrawBoard.mousePressEvent = self.mousePressEvent
        self.mnsitDrawBoard.mouseMoveEvent = self.mouseMoveEvent
        self.mnsitDrawBoard.mouseReleaseEvent = self.mouseReleaseEvent
        self.mnsitDrawBoard.update = self.paintEvent
        self.mnsitDrawBoard.setAutoFillBackground(False)
        self.mnsitDrawBoard.setText("")
        self.mnsitDrawBoard.setObjectName("mnsitDrawBoard")
        self.mnist_perdict = QtWidgets.QLabel(self.mnist_groupBox_4)
        self.mnist_perdict.setGeometry(QtCore.QRect(50, 240, 101, 16))
        self.mnist_perdict.setText("")
        self.mnist_perdict.setObjectName("mnist_perdict")
        self.resnet50_groupBox_5 = QtWidgets.QGroupBox(Dialog)
        self.resnet50_groupBox_5.setGeometry(QtCore.QRect(370, 340, 691, 271))
        self.resnet50_groupBox_5.setObjectName("resnet50_groupBox_5")
        self.resnetLoadImage_pushButton_10 = QtWidgets.QPushButton(self.resnet50_groupBox_5)
        self.resnetLoadImage_pushButton_10.setGeometry(QtCore.QRect(20, 30, 171, 31))
        self.resnetLoadImage_pushButton_10.setObjectName("resnetLoadImage_pushButton_10")
        self.resnetShowImage_pushButton_11 = QtWidgets.QPushButton(self.resnet50_groupBox_5)
        self.resnetShowImage_pushButton_11.setGeometry(QtCore.QRect(20, 70, 171, 31))
        self.resnetShowImage_pushButton_11.setObjectName("resnetShowImage_pushButton_11")
        self.resnetModelShow_pushButton_12 = QtWidgets.QPushButton(self.resnet50_groupBox_5)
        self.resnetModelShow_pushButton_12.setGeometry(QtCore.QRect(20, 110, 171, 31))
        self.resnetModelShow_pushButton_12.setObjectName("resnetModelShow_pushButton_12")
        self.resnetComparisonShow_pushButton_13 = QtWidgets.QPushButton(self.resnet50_groupBox_5)
        self.resnetComparisonShow_pushButton_13.setGeometry(QtCore.QRect(20, 150, 171, 31))
        self.resnetComparisonShow_pushButton_13.setObjectName("resnetComparisonShow_pushButton_13")
        self.resnetInference_pushButton_14 = QtWidgets.QPushButton(self.resnet50_groupBox_5)
        self.resnetInference_pushButton_14.setGeometry(QtCore.QRect(20, 190, 171, 31))
        self.resnetInference_pushButton_14.setObjectName("resnetInference_pushButton_14")
        self.resnet50_graphicsView_2 = QtWidgets.QGraphicsView(self.resnet50_groupBox_5)
        self.resnet50_graphicsView_2.setGeometry(QtCore.QRect(240, 20, 431, 221))
        self.resnet50_graphicsView_2.setObjectName("resnet50_graphicsView_2")
        self.resnet50_perdict = QtWidgets.QLabel(self.resnet50_groupBox_5)
        self.resnet50_perdict.setGeometry(QtCore.QRect(60, 240, 101, 16))
        self.resnet50_perdict.setText("")
        self.resnet50_perdict.setObjectName("resnet50_perdict")
        
        # Create a QPixmap for drawing
        self.pixmap = QPixmap(self.mnsitDrawBoard.size())
        self.pixmap.fill(Qt.black)
        self.mnsitDrawBoard.setPixmap(self.pixmap)
        # Initialize the last point
        self.last_point = QPoint()

        # global load image button handler
        self.globalLoadImage_pushButton.clicked.connect(self.open_global_image)
        
        # Hough Circle Transform section button handler
        self.drawContour_pushButton.clicked.connect(self.drawContour_clicked)
        self.countCoins_pushButton_2.clicked.connect(self.countCoins_clicked)

        # Histogram Equalization section button handler
        self.histogramEqualization_pushButton_3.clicked.connect(self.histogramEqualization_clicked)
        
        # Morphology Operation section button handler
        self.closing_pushButton_4.clicked.connect(self.closing_clicked)
        self.opening_pushButton_5.clicked.connect(self.opening_clicked)
        
        # MNIST section button handler
        self.vggStructure_pushButton_6.clicked.connect(self.vggStructure_clicked)
        self.mnistAccuracyLoss_pushButton_7.clicked.connect(self.mnistAccuracyLoss_clicked)
        self.mnistPredict_pushButton_8.clicked.connect(self.mnistPredict_clicked)
        self.mnistReset_pushButton_9.clicked.connect(self.mnistReset_clicked)
        
        # ResNet30 section button handler
        self.resnetLoadImage_pushButton_10.clicked.connect(self.resnetLoadImage_clicked)
        self.resnetShowImage_pushButton_11.clicked.connect(self.resnetShowImage_clicked)
        self.resnetModelShow_pushButton_12.clicked.connect(self.resnetModelShow_clicked)
        self.resnetComparisonShow_pushButton_13.clicked.connect(self.resnetComparisonShow_clicked)
        self.resnetInference_pushButton_14.clicked.connect(self.resnetInference_clicked)

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Dialog"))
        self.globalLoadImage_pushButton.setText(_translate("Dialog", "Load Image"))
        self.houghCircleTransform_groupBox.setTitle(_translate("Dialog", "1. Hough Circle Transform"))
        self.drawContour_pushButton.setText(_translate("Dialog", "1.1 Draw Contour"))
        self.countCoins_pushButton_2.setText(_translate("Dialog", "1.2 Count Coins"))
        self.houghCircleTransform_label.setText(_translate("Dialog", "There are _ coins in the image."))
        self.histogramEqualization_groupBox_2.setTitle(_translate("Dialog", "2. Histogram Equalization"))
        self.histogramEqualization_pushButton_3.setText(_translate("Dialog", "2. Histogram Equalization"))
        self.morphologyOperation_groupBox_3.setTitle(_translate("Dialog", "3. Morphology Operation"))
        self.closing_pushButton_4.setText(_translate("Dialog", "3.1 Closing"))
        self.opening_pushButton_5.setText(_translate("Dialog", "3.2 Opening"))
        self.mnist_groupBox_4.setTitle(_translate("Dialog", "4. MNIST Classifier Using VGG19"))
        self.vggStructure_pushButton_6.setText(_translate("Dialog", "1. Show Model Structure"))
        self.mnistAccuracyLoss_pushButton_7.setText(_translate("Dialog", "2. ShowAccuracy and Loss"))
        self.mnistPredict_pushButton_8.setText(_translate("Dialog", "3. Predict"))
        self.mnistReset_pushButton_9.setText(_translate("Dialog", "4. Reset"))
        self.resnet50_groupBox_5.setTitle(_translate("Dialog", "5. ResNet50"))
        self.resnetLoadImage_pushButton_10.setText(_translate("Dialog", "Load Image"))
        self.resnetShowImage_pushButton_11.setText(_translate("Dialog", "5.1 Show Images"))
        self.resnetModelShow_pushButton_12.setText(_translate("Dialog", "5.2 Show Model Structure"))
        self.resnetComparisonShow_pushButton_13.setText(_translate("Dialog", "5.3 Show Comparison"))
        self.resnetInference_pushButton_14.setText(_translate("Dialog", "5.4 Inference"))

    def paintEvent(self, event):
        # Override paintEvent to update the label
        painter = QPainter(self.pixmap)
        painter.setPen(QPen(Qt.white, 5, Qt.SolidLine, Qt.RoundCap, Qt.RoundJoin))
        painter.drawEllipse(self.last_point, 5, 5)  # Ensure a single point is drawn
        painter.end()

        self.mnsitDrawBoard.setPixmap(self.pixmap)

    def mousePressEvent(self, event):
        # Handle mouse press event
        if event.button() == Qt.LeftButton:
            self.last_point = event.pos()

    def mouseMoveEvent(self, event):
        # Handle mouse move event
        if event.buttons() & Qt.LeftButton:
            self.last_point = event.pos()
            self.mnsitDrawBoard.update(event)

    def mouseReleaseEvent(self, event):
        # Handle mouse release event
        if event.button() == Qt.LeftButton and self.last_point:
            self.last_point = None
            
    """Starting to code the features"""
    # global Load Image
    def open_global_image(self):
        file_name = QtWidgets.QFileDialog.getOpenFileName(self.globalLoadImage_pushButton, 'open file', '.')
        f_name = file_name[0]

        self.image = cv2.imread(f_name)
    
    """1. Hough Circle Transform """
    def drawContour_clicked(self):
        # Convert the image to grayscale
        gray = cv2.cvtColor(self.image, cv2.COLOR_BGR2GRAY)

        # Apply GaussianBlur to remove noise
        gray_blurred = cv2.GaussianBlur(gray, (5, 5), 0)

        # Use HoughCircles to detect circles
        circles = cv2.HoughCircles(
            gray_blurred, 
            cv2.HOUGH_GRADIENT, dp=1, minDist=40, param1=100, param2=30, minRadius=10, maxRadius=50
        )

        # Define the variable to save processed image
        processed_image = self.image.copy()
        # Define the variable to save circle center image and make it pure black
        centered_image = self.image.copy()
        centered_image[:, :] = [0, 0, 0]
        # Ensure at least some circles were found
        if circles is not None:
            circles = np.uint16(np.around(circles))
            
            # Draw the circles on the original image
            for i in circles[0, :]:
                cv2.circle(processed_image, center=(i[0], i[1]), radius=i[2], color=(0, 255, 0), thickness=2) # Draw the outer circle
                cv2.circle(centered_image, center=(i[0], i[1]), radius=2, color=(255, 255, 255), thickness=3)  # Draw the center of the circles

            # Display the original image, processed image, and circle center image
            cv2.imshow("Original Image", self.image)
            cv2.imshow("Processed Image", processed_image)
            cv2.imshow("Circle Center Image", centered_image)
            cv2.waitKey(0)
            cv2.destroyAllWindows()
        else:
            print("No circles detected.")
            
    def countCoins_clicked(self):
        # Convert the image to grayscale
        gray = cv2.cvtColor(self.image, cv2.COLOR_BGR2GRAY)

        # Apply GaussianBlur to remove noise
        gray_blurred = cv2.GaussianBlur(gray, (5, 5), 0)

        # Use HoughCircles to detect circles
        circles = cv2.HoughCircles(
            gray_blurred, 
            cv2.HOUGH_GRADIENT, dp=1, minDist=40, param1=100, param2=30, minRadius=10, maxRadius=50
        )
        
        # Ensure at least some circles were found
        if circles is not None:
            circles = np.uint16(np.around(circles))
            # Draw the circles on the original image and count the coins
            coin_count = len(circles[0])
            self.houghCircleTransform_label.setText(f"There are {coin_count} coins in the image")
        else:
            self.houghCircleTransform_label.setText("There are 0 coins in the image")

    """2. Histogram Equalization """
    def histogramEqualization_clicked(self):
        self.image = cv2.cvtColor(self.image, cv2.COLOR_BGR2GRAY)
        # Equalized image from opencv
        equalized_image_opencv = cv2.equalizeHist(self.image)
        
        # Equalized image by manual
        def histogram_equalization_manual(image):
            hist, bins = np.histogram(image.flatten(), 256, [0, 256])
            pdf = hist / np.sum(hist)
            cdf = np.cumsum(pdf) * 255
            lookup_table = np.round(cdf).astype(np.uint8)
            equalized_image = lookup_table[image]
            return equalized_image

        equalized_image_manual = histogram_equalization_manual(self.image)
        
        # Display original and equalized (by opencv and manual) images
        plt.subplot(2, 3, 1)
        plt.imshow(self.image, cmap='gray')
        plt.title('Original Image')
        
        plt.subplot(2, 3, 2)
        plt.imshow(equalized_image_opencv, cmap='gray')
        plt.title('Equalized (OpenCV)')
        
        plt.subplot(2, 3, 3)
        plt.imshow(equalized_image_manual, cmap='gray')
        plt.title('Equalized (Manual)')

        # Display histograms
        def display_histogram(image, title):
            plt.hist(image.ravel(), bins=256, range=[0, 256], alpha=0.5)
            plt.title(title)
            plt.xlabel('Pixel Value')
            plt.ylabel('Frequency')
            plt.show()
            
        plt.subplot(2, 3, 4)
        display_histogram(self.image, 'Histogram (Original)')
        plt.subplot(2, 3, 5)
        display_histogram(equalized_image_opencv, 'Histogram (Equalized OpenCV)')
        plt.subplot(2, 3, 6)
        display_histogram(equalized_image_manual, 'Histogram (Equalized Manual)')
        
    """3. Morphology Operation """
    def closing_clicked(self):
        gray_image = cv2.cvtColor(self.image, cv2.COLOR_BGR2GRAY)
        _, binary_image = cv2.threshold(gray_image, 127, 255, cv2.THRESH_BINARY)

        kernel_size = 3
        # Pad the image with zeros based on the kernel size (K=3)
        pad_size = kernel_size // 2  # Padding size for a 3x3 kernel
        padded_image = np.pad(binary_image, pad_size, mode='constant', constant_values=0)
        structuring_element = np.ones((kernel_size, kernel_size), dtype=np.uint8)

        # Perform dilation
        dilated_image = self.dilation(padded_image, structuring_element)

        # Perform erosion
        eroded_image = self.erosion(dilated_image, structuring_element)

        closing_result = eroded_image[pad_size:-pad_size, pad_size:-pad_size]

        plt.imshow(closing_result, cmap='gray')
        plt.title('Closing Operation Result')
        plt.show()

    def opening_clicked(self):
        gray_image = cv2.cvtColor(self.image, cv2.COLOR_BGR2GRAY)
        _, binary_image = cv2.threshold(gray_image, 127, 255, cv2.THRESH_BINARY)

        kernel_size = 3
        # Pad the image with zeros based on the kernel size (K=3)
        pad_size = kernel_size // 2  # Padding size for a 3x3 kernel
        padded_image = np.pad(binary_image, pad_size, mode='constant', constant_values=0)
        structuring_element = np.ones((kernel_size, kernel_size), dtype=np.uint8)

        # Perform erosion
        eroded_image = self.erosion(padded_image, structuring_element)

        # Perform dilation
        dilated_image = self.dilation(eroded_image, structuring_element)

        opening_result = eroded_image[pad_size:-pad_size, pad_size:-pad_size]

        # Display the result
        plt.imshow(opening_result, cmap='gray')
        plt.title('Opening Operation Result')
        plt.show()
    
    def erosion(self, image, kernel):
        result = np.zeros_like(image) # initilize the zero n-d array with the image size
        pad_size = kernel.shape[0] // 2
        for i in range(pad_size, image.shape[0] - pad_size):
            for j in range(pad_size, image.shape[1] - pad_size):
                if np.min(image[i - pad_size:i + pad_size + 1, j - pad_size:j + pad_size + 1] * kernel) > 0:
                    result[i, j] = 255 
        return result

    def dilation(self, image, kernel):
        result = np.zeros_like(image) # initilize the zero n-d array with the image size
        pad_size = kernel.shape[0] // 2
        for i in range(pad_size, image.shape[0] - pad_size):
            for j in range(pad_size, image.shape[1] - pad_size):
                if np.max(image[i - pad_size:i + pad_size + 1, j - pad_size:j + pad_size + 1] * kernel) == 255:
                    result[i, j] = 255
        return result

        
    """4. Training a MNIST Classifier Using VGG19 with BN """
    def vgg19_bn(self):
        vgg19 = models.vgg19_bn()
        # little adjustments to the model due to the mnist dataset
        vgg19.features[0] = nn.Conv2d(1, 64, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1))
        return vgg19
    
    def vggStructure_clicked(self):
        torchsummary.summary(self.vgg19, (1, 32, 32))
        
    def mnistAccuracyLoss_clicked(self):
        image = cv2.imread('training_history.png')
        cv2.imshow('Training history', image)
        
    def mnistPredict_clicked(self):
        self.mnsitDrawBoard.pixmap().save('test_case.jpg')
        self.drawn_image = Image.open('test_case.jpg')
        # Resize the image to 28x28
        resized_image = self.drawn_image.resize((28, 28), Image.ANTIALIAS)

        # Convert the image to grayscale
        grayscale_image = resized_image.convert("L")

        # Normalize pixel values
        normalized_values = np.array(grayscale_image) / 255.0

        # Flatten the image
        flattened_image = normalized_values.flatten()
        # Convert the NumPy array back to a PIL Image => An mnist-like image !
        pil_image = Image.fromarray((normalized_values * 255).astype(np.uint8), mode='L')
        
        model = self.vgg19
        model.load_state_dict(torch.load('model/best_vgg19_bn_mnist.pth'))
        
        transform = transforms.Compose([
            transforms.Resize((32, 32)),
            transforms.ToTensor(),
            transforms.Normalize((0.5,), (0.5,))
        ])

        image_tensor = transform(pil_image).view(1, 1, 32, 32) 
        
        # Perdict the image by given model
        with torch.no_grad():
            model.eval()
            outputs = model(image_tensor)
            probabilities = torch.nn.functional.softmax(outputs[0], dim=0)
            predicted = torch.argmax(probabilities).item()
            class_label = f'Predicted = {classes[predicted]}'
            self.mnist_perdict.setText(class_label)
            
            plt.figure()
            plt.bar(classes, probabilities[:len(classes)].numpy())
            plt.xticks(classes)
            plt.xlabel('Class')
            plt.ylabel('Probability')
            plt.title('Result of the perdiction of the model')
            plt.show()

    def mnistReset_clicked(self):
        self.pixmap.fill(Qt.black)
        self.mnsitDrawBoard.setPixmap(self.pixmap)
        # Clear the drawn image variable
        self.drawn_image = None
            
    """5. Train a Cat-Dog Classifier Using ResNet50 """ 
    # Build ResNet50 model
    def resnet50_custom(self, num_classes=1):
        resnet50 = models.resnet50()
        
        # Replace the output layer
        in_features = resnet50.fc.in_features
        resnet50.fc = nn.Sequential(
            nn.Linear(in_features, num_classes),
            nn.Sigmoid()
        )
        
        return resnet50
    
    def resnetLoadImage_clicked(self):
        file_name = QtWidgets.QFileDialog.getOpenFileName(self.globalLoadImage_pushButton, 'open file', '.')
        f_name = file_name[0]
        
        # resnet image
        self.resnet_image = cv2.imread(f_name)
        
        # Show the chosen image to the `QGraphicsView`
        scene = QtWidgets.QGraphicsScene()
        scene.setSceneRect(0, 0, 200, 200)
        img = QtGui.QPixmap(f_name)
        img = img.scaled(128,128)
        scene.addPixmap(img)
        self.resnet50_graphicsView_2.setScene(scene)
        
    def resnetShowImage_clicked(self):
        inference_dataset_path = 'inference_dataset'
        
        # Create the custom dataset
        custom_dataset = CustomDataset(root_dir=inference_dataset_path, transform=transform_resnet)

        # Create a DataLoader for the dataset
        data_loader = DataLoader(custom_dataset, batch_size=1, shuffle=False)

        # Iterate over the dataset and display images
        for cat_img, cat_label, dog_img, dog_label in data_loader:
            # Display cat image
            plt.figure()
            plt.subplot(1, 2, 1)
            plt.imshow(cat_img.squeeze().permute(1, 2, 0))
            plt.title('Cat')
            plt.axis('off')

            # Display dog image
            plt.subplot(1, 2, 2)
            plt.imshow(dog_img.squeeze().permute(1, 2, 0))
            plt.title('Dog')
            plt.axis('off')

            plt.show()
    
    def resnetModelShow_clicked(self):
        torchsummary.summary(self.resnet50, (3, 224, 224)) 
        
    def resnetComparisonShow_clicked(self):
        image = cv2.imread('validation_accuracies_comparison.png')
        cv2.imshow('Validation Accuracies Comparison', image)
        
    def resnetInference_clicked(self):
        model = self.resnet50
        model.load_state_dict(torch.load('model/resnet50_model_with_erase.pth'))

        image_tensor = transform_resnet(Image.fromarray(self.resnet_image)).unsqueeze(0)
        
        # Perdict the image by given model
        with torch.no_grad():
            model.eval()
            outputs = model(image_tensor)
            if (outputs > 0.5):
                self.resnet50_perdict.setText("Predict: Dog")
            else:
                self.resnet50_perdict.setText("Predict: Cat")


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Dialog = QtWidgets.QDialog()
    ui = Ui_Dialog()
    ui.setupUi(Dialog)
    Dialog.show()
    sys.exit(app.exec_())
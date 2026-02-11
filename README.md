# TinyML Healthcare Monitor

A TinyML-based real-time healthcare monitoring system that uses sensor data and on-device machine learning to classify a person’s health condition as NORMAL, STRESS, or CRITICAL.

The system performs inference directly on Arduino, enabling fast response, low latency, and no dependency on cloud computation.


# Features

• Real-time health monitoring  
• On-device ML inference (TinyML)  
• Automatic health classification  
• LCD status display  
• Alert system for critical conditions  
• Fully embedded solution  


# Project Architecture

Sensor Data → Arduino → TinyML Model → Health Prediction → LCD Display → Alert


# Components Used

- Arduino Uno  
- Temperature Sensor  
- Flex Sensor  
- Heart Rate Sensor (simulated)  
- PIR Motion Sensor  
- I2C LCD Display  
- Buzzer  


# Methodology

1. Sensor data was collected using Arduino and exported as CSV.
2. The dataset was trained using Edge Impulse to build a classification model.
3. The trained TinyML model was deployed on Arduino.
4. Real-time sensor readings are passed into the model.
5. The system predicts the health condition and displays it on the LCD.


# Dataset

The dataset used for training is available here: dataset/health_dataset_150.csv

It contains sensor readings labeled into three categories:

NORMAL  
STRESS  
CRITICAL  


# Training

Model training was performed using Edge Impulse.

Training notebook: training/training_notebook.ipynb



# Arduino Implementation

The deployed Arduino code can be found here: arduino/tinyml_health_monitor.ino



# Results

The system successfully classifies health conditions in real-time and triggers alerts during critical states.

TinyML enabled efficient on-device prediction without requiring internet connectivity.


# Future Improvements

• Integrate real biomedical sensors  
• Improve dataset size  
• Deploy on wearable hardware  
• Add mobile app connectivity  


# Author

Built as part of a TinyML academic project.






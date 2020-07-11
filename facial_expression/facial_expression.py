import cv2

# Loading the cascades
face_cascade = cv2.CascadeClassifier('C:\\Users\\makka\\Desktop\\facial_expression\\haarcascade_frontalface_default.xml') # We load the cascade for the face.
smile_cascade = cv2.CascadeClassifier('C:\\Users\\makka\\Desktop\\facial_expression\\haarcascade_smile.xml')

# viola_jones detects first in black & white"gray" img then apply it on color images"frame"
def detect(gray, frame):
    

    #detectMultiScale >> perform the detection
    faces = face_cascade.detectMultiScale(gray, 1.3, 3)
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2) #bgr
        roi_gray = gray[y:y+h, x:x+w]
        roi_color = frame[y:y+h, x:x+w]
        
        
        smiles = smile_cascade.detectMultiScale(roi_gray, 1.9, 9)     #detect on the face region, no need to scan the whole image
        for (sx, sy, sw, sh) in smiles:
            cv2.rectangle(roi_color, (sx, sy), (sx+sw, sy+sh), (0, 255, 0), 2)
            print("Smile detected! ")


    return frame


video_capture = cv2.VideoCapture(0)

while True:
    
    
    #to read from webcam
    _,frame =video_capture.read()
    gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)   
    
    #test images
    #frame = cv2.imread('C:\\Users\\makka\\Desktop\\facial_expression\\hello.jpg',1) 
    #frame = cv2.imread('C:\\Users\\makka\\Desktop\\facial_expression\\Cole_Calistra.jpeg',1) 
    #frame = cv2.imread('C:\\Users\\makka\\Desktop\\facial_expression\\Girl-Smiling.jpg',1) 
    #frame = cv2.imread('C:\\Users\\makka\\Desktop\\facial_expression\\sad.jpg',1)
     
    #gray = cv2.imread('C:\\Users\\makka\\Desktop\\facial_expression\\hello.jpg', cv2.IMREAD_GRAYSCALE)
    #gray = cv2.imread('C:\\Users\\makka\\Desktop\\facial_expression\\Cole_Calistra.jpeg', cv2.IMREAD_GRAYSCALE) 
    #gray = cv2.imread('C:\\Users\\makka\\Desktop\\facial_expression\\Girl-Smiling.jpg', cv2.IMREAD_GRAYSCALE) 
    #gray = cv2.imread('C:\\Users\\makka\\Desktop\\facial_expression\\sad.jpg', cv2.IMREAD_GRAYSCALE)
    

    img = detect(gray, frame)
    cv2.imshow('happy or not!', img)
    

    #if we click "q" the webcam should stop
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

#stop the webcam    
video_capture.release()
cv2.destroyAllWindows()

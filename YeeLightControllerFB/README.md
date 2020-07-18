# Yee Lamp Controller in C++
Using Firebase Authenticaiton, Cloud Functions and Realtime Database to implement the OAuth and SEE Protocol, writen in C++/QT Library and Node JS
to control Yee Light Lamp Device. 

- Server is writen in NodeJS to handle the Auth server capabilities for login, generate Firebase auth token and token validation 
- Client is wirten in C++/QT Library to handle as a SEE EventSource client when there is a suser publish new command in Firebase Realtime Database. Base on the command which is published this c++ client will send it to the Yee Lamp to control the Lamp Colour, Brightness, Power or Temperature



# SSE Server

## Getting Started

go to project folder [server]

then login to firebase

$firebase login

Login to browser


## Init project with firebase project for the first time project's created

$firebase init functions

- Select existing project, which's already been created in firebase.
- select javascript, for this project in project set up,and then use ESLint
- install dependecy with npmm, by chosing y, the project folder [server/functions] will be generated and firebase.json will be created in project folder[server/firebase.json].

go to  functions folder then edit index.js as needed, 
or use another file name  like server.js then edit the main tag in package.json  

$cd functions

[start coding and have fun]

add required dependency 

$npm i express cors

this will add also the dependencies in package.json



## Test the project in local browser

$firebase emulators:start


- Test the apps by accessing at local browser  (http://localhost:3000/login)


## Deploy the poject to cloud function

$ firebase deploy --only functions


## Test the apps

Test the publisher using below url to login, get the firebase token, and validate the token

- Test the apps by accessing (https://us-central1-[your-project-name].cloudfunctions.net/app/login)
- Test the apps by accessing (https://us-central1-[your-project-name].cloudfunctions.net/app/validatetoken)



# SSE Client

To test the end to End scenario we use 2 file, as the listerner and as the publishser 

## Run the SSE NodeJs Server 

Run the server in local emulator or deploy it to the Firebase cloud, go to functions folder

$firebase emulators:start or firebase deploy --only functions

## Build the project usisng
./buil.sh

## Run the listenner 
./runlistener.sh

## Send a command using
./runpublisher.sh


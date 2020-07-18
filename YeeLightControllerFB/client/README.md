# Yee Light Oauth and SSE Event Source Client Handler 

this project is to build a controller for Yee Light Device using QT library on mac OS
- Server is writen in NodeJS to handle the Auth server capabilities for login, generate Firebase auth token and token validation 
- Client is wirten in C++/QT Library to handle as a SEE EventSource client when there is a suser publish new command in Firebase Realtime Database. Base on the command which is published this c++ client will send it to the Yee Lamp to control the Lamp Colour, Brightness, Power or Temperature



create project folder 

src/
build/
header/
makefile


build the project usisng
./buil.sh

run the listenner 
./runlistener.sh

send a command using
./runpublisher.sh


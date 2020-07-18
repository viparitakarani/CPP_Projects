#!/bin/bash

echo "Input Firebase Cloud Functions host:"
#read host
host=https://us-central1-[your project name].cloudfunctions.net
#http://localhost
echo "Input Firebase Cloud Functions port:"
#read port
port="443"
#192.168.1.3
echo "Iput Comandid [0=change the lamp colour, 1= change lamp temperatur, 2= change lamp brightness,3= change lamp power,4= start lamp flow,4= stop lamp flow]:"
read commandid
#commandid=1
echo "Iput Your E-mail:"
read email
echo "Iput Your Password:"
read pass

#./build/release/ClientSSE sendevent $host $port $commandid
./build/release/ClientSSE.app/Contents/MacOS/ClientSSE sendevent $host $port $commandid $email $pass
 



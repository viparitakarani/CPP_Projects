echo "Input Firebase Cloud Functions host:"
#read host
host=https://us-central1-[your project name].cloudfunctions.net
#http://localhost
echo "Input Firebase Cloud Functions port:"
#read port
port="443"
#192.168.1.3
echo "Iput IP of the Yee Lamp:"
#read lampip
lampip=192.168.1.3
echo "Iput Your E-mail:"
read email
#email=test@gmail.com
echo "Iput Your Password:"
read pass
#pass=pass123
#./build/release/ClientSSE listening $host $port $lampip
./build/release/ClientSSE.app/Contents/MacOS/ClientSSE listening $host $port $lampip $email $pass

#https://us-central1-yeesse-b511a.cloudfunctions.net/app/login
#https://us-central1-yeesse-b511a.cloudfunctions.net/app/sendevent
#https://us-central1-yeesse-b511a.cloudfunctions.net/app/requeststreams
#
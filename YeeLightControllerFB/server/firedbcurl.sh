#!/bin/sh
result=$(curl -X POST -d '{"email":"test@gmail.com","pass":"test123"}' -H 'Content-Type: application/json' https://us-central1-[project name].cloudfunctions.net/app/login)
curl -X POST -i  -H 'Content-Type: application/json' -H 'Accept: application/json' -H "Authorization: Bearer $result" https://us-central1-[project name].cloudfunctions.net/app/validatetoken

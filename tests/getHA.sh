#!/bin/bash

API_KEY=$(pass gluttony/homeassistant/API)
IP_ADDR="192.168.1.104"

curl -X GET \
    -H "Authorization: Bearer $API_KEY" \
    -H "Content-Type: application/json" \
    -o out.json \
    http://$IP_ADDR:8123/api/states

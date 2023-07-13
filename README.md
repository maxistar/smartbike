# Bicycle Tracker Project

LiliGo T-CALL-SIM800L module can be used or this project.

# Post Status

## Post to local server
```shell
curl -X POST http://localhost:8080/api/status/ -H "Content-Type: application/json" -d '{"deviceID": 1, "powerSource":"USB", "batteryStatus":"CHARGING", "battery":44, "batteryInfo":{"test":"value"}, "version":1}'
```

## Post to remove server
```shell
curl -X POST http://bike.x.maxistar.me/api/status/ -H "Content-Type: application/json" -d '{"deviceID": 1, "powerSource":"USB", "batteryStatus":"CHARGING", "battery":44, "batteryInfo":{"test":"value"}, "version":1}'
```

## get latest status
http://bike.x.maxistar.me/api/info/latest

# get firmware version
http://bike.x.maxistar.me/api/firmware/latest

# get firmware                                   
wget --server-response http://bike.x.maxistar.me/firmware/3/smartbike.ino.esp32.bin

upload new firmware
1. create folder on the server
2. export firmware
3. upload firmware
   scp /home/maxim/projects/smartbike/fahrrad-tracker/firmware/sim7000/sim7000.ino.esp32.bin maxistar.ru:/home/maxim/projects/fahrrad-tracker/server/web/firmware/14
4. in the server `insert into firmware VALUES (15, 15, 15, 15, 15);`

http://bike.x.maxistar.me/firmware/11/sim7000.ino.esp32.bin

scp /home/maxim/projects/smartbike/fahrrad-tracker/firmware/sim7000/sim7000.ino.esp32.bin maxim@maxistar.ru:/home/maxim/projects/fahrrad-tracker/server/web/firmware/14


## Links ##
- [Random Nerd Tutorial](https://randomnerdtutorials.com/esp32-sim800l-publish-data-to-cloud/)
- [Nice DIY monitoring project](https://www.instructables.com/id/ESP32-SIM800L-and-Barrier-Sensor/)
- [How to start article in russian](http://www.bizkit.ru/2019/02/05/11989/)
- [LiliGo T-CALL-SIM800L Project Repo](https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800L/issues/29)
- [пример что можно использовать для управления питанием.](https://gist.github.com/me-no-dev/)

## Run on production

> forever start dist


## New firmware upload
Export new firmware
Move to the new folder

## Run tests


Run emulator:
emulator -avd Nexus_S_API_28
yarn start
yarn android

## Docker

```
cd docker
docker build -t arduino01 .

docker run -it --rm -v "$sketch_folder":/usr/src/sketch arduino01 ls

docker run -it --rm -v "$(pwd)":/usr/src/sketch arduino01 bash \
-c 'arduino-cli core update-index --additional-urls="https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json" && arduino-cli core install esp32:esp32 --additional-urls="https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json"'


```





```
cd firmware
docker run -it --rm -v "$(pwd)":/usr/src/sketch maxistar/arduino-esp32
```

using on mac
````
docker build -t maxistar/arduino-esp32-ci --platform linux/amd64 .
````

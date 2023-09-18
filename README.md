# Bicycle Tracker Project

The version based on [LiliGo T-CALL-SIM800L](https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800) module can be used for this project.
The module does not have GSM module, so if you want to use satellite positioning then NEO-7M module additionally needed.

The version based on [LilyGO-T-SIM7000G](https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G) module can be used for this project.

Additionally following components can be used:

- [Solar Battery](https://amzn.eu/d/6o92Us3)
- [Waterproof electical plug](https://amzn.eu/d/hH6DRAf)

This project contents backed service, android application, arduino firmware.
There is also iOS app exists.

The project allows getting telemetry from your bike which includes GPS location, battery level, insulation level and some other utility data.

In order to make the project independent of external power supply the telemetry is being sent every 1-2 hours. Which allows battery changed from relatively small solar element.


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
   scp /home/maxim/projects/smartbike/fahrrad-tracker/firmware/sim7000/sim7000/sim7000.ino.esp32.bin maxistar.ru:/home/maxim/projects/fahrrad-tracker/server/web/firmware/26
4. in the server `insert into firmware VALUES (15, 15, 15, 15, 15);`

http://bike.x.maxistar.me/firmware/11/sim7000.ino.esp32.bin

scp /home/maxim/projects/smartbike/fahrrad-tracker/firmware/sim7000/sim7000.ino.esp32.bin maxim@maxistar.ru:/home/maxim/projects/fahrrad-tracker/server/web/firmware/23


## Run on production

in order to use server part on production you can use [forever](https://www.npmjs.com/package/forever) utility.

> `forever start dist`

check if the service is running:

> `forever list`


## New firmware upload

Export new firmware
Move to the new folder

## Run tests


Run emulator:
emulator -avd Nexus_S_API_28
yarn start
yarn android

## Docker

The firmware can be compiled using docker image. At the moment this feature is used only in CI to test the firmware.

```
cd docker
docker build -t arduino01 .

docker run -it --rm -v "$sketch_folder":/usr/src/sketch arduino01 ls

docker run -it --rm -v "$(pwd)":/usr/src/sketch arduino01 bash \
-c 'arduino-cli core update-index --additional-urls="https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json" && arduino-cli core install esp32:esp32 --additional-urls="https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json"'


```

```
cd firmware/sim7000
docker run -it --rm -v "$(pwd)":/usr/src/sketch maxistar/arduino-esp32
```

using on mac
````
docker build -t maxistar/arduino-esp32-ci --platform linux/amd64 .
````

## Links

- [Random Nerd Tutorial](https://randomnerdtutorials.com/esp32-sim800l-publish-data-to-cloud/)
- [Nice DIY monitoring project](https://www.instructables.com/id/ESP32-SIM800L-and-Barrier-Sensor/)
- [How to start, article in russian](http://www.bizkit.ru/2019/02/05/11989/)
- [LiliGo T-CALL-SIM800L Project Repo](https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800L/issues/29)
- [пример что можно использовать для управления питанием.](https://gist.github.com/me-no-dev/)

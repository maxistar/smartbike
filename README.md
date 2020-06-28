# Bicycle Tracker Project

LiliGo T-CALL-SIM800L module can be used or this project.

# Post Status
curl -X POST http://localhost:8080/api/status/ -H "Content-Type: application/json" -d '{"powerSource":"USB", "batteryStatus":"CHARGING", "battery":44}'

#get firmware                                   
wget --server-response http://localhost:8080/firmware/1/smartbike.ino.esp32.bin


## Links ##
- [Random Nerd Tutorial](https://randomnerdtutorials.com/esp32-sim800l-publish-data-to-cloud/)
- [Nice DIY monitoring project](https://www.instructables.com/id/ESP32-SIM800L-and-Barrier-Sensor/)
- [How to start article in russian](http://www.bizkit.ru/2019/02/05/11989/)
- [LiliGo T-CALL-SIM800L Project Repo](https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800L/issues/29)
- [пример что можно использовать для управления питанием.](https://gist.github.com/me-no-dev/)

## Run on production

> forever start dist

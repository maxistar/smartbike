FROM python:3.7-slim

RUN apt-get update && apt-get install -y curl openssh-client
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/usr/bin sh
RUN arduino-cli core update-index --additional-urls="https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json" && \
    arduino-cli core install esp32:esp32 --additional-urls="https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json"

RUN pip install --no-cache-dir pyyaml pyserial
COPY compile.py /usr/src/app/

WORKDIR /usr/src/sketch
CMD [ "python", "-u", "/usr/src/app/compile.py" ]

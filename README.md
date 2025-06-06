# ESP32 Temperature Monitor with MQTT and TM1638 Display

이 프로젝트는 ESP32 보드를 사용해 DHT11 센서로 온도를 측정하고, 그 값을 MQTT 브로커로 전송하며, TM1638 LED 모듈을 통해 실시간으로 온도를 표시하는 간단한 IoT 시스템입니다.

## 주요 기능

- DHT11 센서로 현재 온도(섭씨)를 측정
- TM1638 모듈에 "ONDO.XX.XX" 형식으로 온도 출력
- MQTT 브로커에 주기적으로 온도 데이터 전송

## 사용한 부품

- ESP32 WROOM
- DHT11 온습도 센서
- TM1638 LED+버튼 모듈
- Wi-Fi
- MQTT 브로커

## 핀 연결 

| 장치      | ESP32 핀 | 설명         |
|-----------|-----------|--------------|
| DHT11     | GPIO15    | 데이터핀      |
| TM1638 STB| GPIO18    | STROBE 핀     |
| TM1638 CLK| GPIO19    | CLOCK 핀      |
| TM1638 DIO| GPIO21    | 데이터핀      |

> ⚠GPIO15는 ADC2 채널이지만, DHT11은 디지털 방식이라 Wi-Fi 간섭 문제는 없음.

## MQTT 설정

- 브로커: `broker.mqtt-dashboard.com`
- 발행 토픽: `ESP32/LAAS/TEMP/YSH`

## 동작

1. 부팅 후 Wi-Fi 연결
2. DHT11 센서에서 온도 값 읽기
3. TM1638 모듈에 온도 표시 (`ONDO.25.32` 형식)
4. MQTT 브로커에 현재 온도 전송

## 추가 설치 라이브러리

- `PubSubClient` (MQTT)
- `TM1638plus`



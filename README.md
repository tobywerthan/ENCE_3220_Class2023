# **ENCE_3220_Class2023 Phase D**
Phase D of Embedded System tasked students to design their own PCB using the STM32f407G and the STM32CubeIDE. A friend of mine recently asked me if I could design a deivce that would help him monitor the temperature in his bearded dragon's enclosure. This project seemed like the perfect opportunity to begin designing such a device. 

## Project Requirements
The requirements for this temperature monitor aren't complex. The system must be able to read the current temperature, additional data is a plus. The system must also be able to display that data in a readable manner. The system must utilize the STM32F407G Discovery Board, and the software must be developed in the STM32CubeIDE. In summary, the system requirements are as follows: 
- Read temperature
- Display temperature in real time
- Utilize the STM32F407G Discovery Board
- Software developed in the STM32CubeIDE

## System Design
The design for this system includes the STM32F407G Discovery Board, an lcd display, and a temperature sensor. A block diagram of key components and their pin connections to the STM32F4. 

![BlockDiagram](https://github.com/tobywerthan/ENCE_3220_Class2023/assets/55803740/67a1513a-c1a0-4130-bfb1-48bb881af1c0)

## Components Selection
Key components for this system include the LCD1602A display, the DHT11 temperature and humidity sensor, and the STM32F4 discovery board. A full list of the required components can be seen in the table below. 

| References  | Value       | Footprint   |
| :-----------: | :-----------: | :-----------: |
| U2 | DHT11 | Connector_PinSocket_2.54mm:PinSocket_1x03_P2.54mm_Vertical_SMD_Pin1Right |
| U1 | LCD1602A | Connector_PinSocket_2.54mm:PinSocket_1x16_P2.54mm_Vertical_SMD_Pin1Right |
| J1 & J2 | STM32F4 Discovery Board | 2x Connector_PinSocket_2.54mm:PinSocket_2x25_P2.54mm_Vertical |
| RV1 | 1000Ω | Connector_PinSocket_2.54mm:PinSocket_1x05_P2.54mm_Vertical_SMD_Pin1Right |
| R1 | 910Ω | Resistor_SMD:R_0603_1608Metric |
| C1 | 1mF | Capacitor_SMD:C_0603_1608Metric |

## Build Prototype

![IMG_0870](https://github.com/tobywerthan/ENCE_3220_Class2023/assets/55803740/7569cad7-967c-473b-aba5-546d0cd26ed9)

## STM32 Shield Design
The goal for this project was to develop a shield for the STM32F4 that implements the designed system. 

### Schematic
The schematic for this system contains three modules. The modules are categorized by component: "LCD1602A Pinout", "DHT11 Pinout", and "STM32 Discovery board Pinout". 

#### LCD Display

![image](https://github.com/tobywerthan/ENCE_3220_Class2023/assets/55803740/8fba1ba2-90f5-4654-acfb-c5c67ea91418)

#### DHT11

![image](https://github.com/tobywerthan/ENCE_3220_Class2023/assets/55803740/18c32273-0055-41af-b8e5-ca45f9912bb7)

#### STM32F4

![image](https://github.com/tobywerthan/ENCE_3220_Class2023/assets/55803740/5bc7af28-0ada-4f12-b00e-fd8c3a3ebd46)

### PCB
This shield was designed so that the componenets used during prototyping could be directly placed on the PCB using pin socket connectors instead of surface mounting components. The main concern when designing the PCB was to allocate enough space for the components once they are connected to the pin sockets. This was managed through some crude measurements as well as trial and error. 

#### KiCad

![PCB](https://github.com/tobywerthan/ENCE_3220_Class2023/assets/55803740/00986a72-d583-4c47-842b-e8ab2e83f396)

#### 3D Render

![PCB_3D_Render](https://github.com/tobywerthan/ENCE_3220_Class2023/assets/55803740/721ff4a9-9a1c-46c5-99fe-5079a9a311f5)

#### 3D Render with Components 

![PCB_3D_Render_With_Components](https://github.com/tobywerthan/ENCE_3220_Class2023/assets/55803740/7a041e28-2be2-4a8f-9380-b9b233bb2609)

## Software Development
Both the LCD display and the temperature sensor require delays in units of microseconds in order to function correctly. Unfortunately, the delay provided by the HAL library has a minimum of 1 millisecond. To achieve a delay in microseconds, the following code is implemented in the library for both sensors:
```
void microDelay (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&timer, 0);
	while (__HAL_TIM_GET_COUNTER(&timer) < us);
}
```
The timers used for the micro delays are timer1 and timer2 sourced by the internal clock of the STM32. Both timers share a prescaler value of 83 and a counter period of 4095. The LCD library provided functions for clearing the display, moving the cursor on the display, and writing strings to the display. Every loop execution, the LCD displays the temperature on the top row, and then the humidity on the bottom row. The LCD needs to clear every loop execution as the values update, this forces the loop to implement a delay so that the value can display befor being cleared. Because the LCD displays requires a delay, the rate at which the temperature sensor is read needs to be faster than that delay. In this case, the delay for the lcd was 1 second. Therefore, a timer driven interrupt is implemented using timer6. This interrupt occurs every half second, which is plenty fast enough considering the display rate of the LCD display. Each time the interrupt occurs, the temperature and humidity values are read and stored. A flow chart of the program can be seen below:

![Flowchart](https://github.com/tobywerthan/ENCE_3220_Class2023/assets/55803740/c28a7c41-6b64-42db-b7cd-be5866f3170b)

## Future Iterations

While this project is practical, this kind of device already exists. A way to improve this design would be to use the ESP8266 microchip to send alerts to a certain device once the temperature has reached a specified threshold. Additionally, adding a UVB sensor would give the user more information about the enviornment. Powering the system with a battery would allow it to be portable. Because the device is meant to detect high temperatures and UVB, charging the battery through solar might not be a bad idea either. 

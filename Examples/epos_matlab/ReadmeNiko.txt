Dieses Projekt soll zum Testen der Can-Kommunikation und das Verfahrens des Motors mittels PPM OHNE simulink model verwendet werden.

Interessant -> Funktioniert tx_callback(int ?,..-)

Motor bewegt sich im PPM modus

west build -b stm32f4_disco .\zephyr\Medit\epos_matlab
west flash

python3 .\Downloads\pythonploter2.py
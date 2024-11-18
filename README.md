# CPS_Smartlock
Projeto para a cadeira de Ciber Phisical Systems da Unifor 2024.2


# comandos ESP32 > Node-red
- unifor/cps/789456/esp/state_changed
	payload: Booleano


- unifor/cps/789456/new_card
	payload: String
	UUID do cartão

# Comandos Node-red > ESP32
- unifor/cps/789456/change_card_level
	{"card": "03 03 03 03", "level": 1}

- unifor/cps/789456/unlock_access
	payload vazio

- unifor/cps/789456/change_time
	payload: Inteiro
	(milisegundos)

- unifor/cps/789456/access_level
	payload: Booleano

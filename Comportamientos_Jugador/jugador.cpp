#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;
	int a;
	
	// Aquí se hace el reset del juego
	if(sensores.reset){
		reset();
	}

	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Noreste" << endl; break;
		case 2: cout << "Este" << endl; break;
		case 3: cout << "Sureste" << endl; break;
		case 4: cout << "Sur " << endl; break;
		case 5: cout << "Suroeste" << endl; break;
		case 6: cout << "Oeste" << endl; break;
		case 7: cout << "Noroeste" << endl; break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;
	

	// Actualización de las variables de estado
	switch (last_action){
		case actFORWARD:
			if(!sensores.colision){
				// Actualización en caso de avanzar
				switch(current_state.brujula){
					case norte: current_state.fil--; break;
					case noreste: current_state.fil--; current_state.col++; break;
					case este: current_state.col++; break;
					case sureste: current_state.fil++; current_state.col++; break;
					case sur: current_state.fil++; break;
					case suroeste: current_state.fil++; current_state.col--; break;
					case oeste: current_state.col--; break;
					case noroeste: current_state.fil--; current_state.col--; break;
      			}
			};		
			break;
		case actTURN_SL:
			// Actualización en caso de girar 45º a la izquierda
			a =  current_state.brujula;
			a = (a+7)%8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
		case actTURN_SR:
			// Actualización en caso de girar 45º a la derecha
			a =  current_state.brujula;
			a = (a+1)%8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
		case actTURN_BL:
			// Actualización en caso de girar 135º a la izquierda
			a =  current_state.brujula;
			a = (a+6)%8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
		case actTURN_BR:
			// Actualización en caso de girar 135º a la derecha
			a =  current_state.brujula;
			a = (a+2)%8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
	}

	if(sensores.nivel == 1 and !bien_situado){
		current_state.brujula = sensores.sentido;
	}

	if((sensores.terreno[0] == 'G' or sensores.nivel == 0) and !bien_situado){
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
	}

	if(bien_situado){
		PonerTerrenoEnMatriz(sensores.terreno, current_state, mapaResultado);
	}

	// Decidir la nueva acción
	if((sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' or 
	    sensores.terreno[2] == 'G' or sensores.terreno[2] == 'D' or 
		sensores.terreno[2] == 'K' or sensores.terreno[2] == 'X' or 
		(sensores.terreno[2] == 'B' and zapatillas) or 
		(sensores.terreno[2] == 'A' and bikini) and 
		(sensores.superficie[2] == '_'))){
		accion = actFORWARD;
	} else if(!girar_derecha){
		accion = actTURN_SL;
		girar_derecha = (rand()%2 == 0);
	} else {
		accion = actTURN_SR;
		girar_derecha = (rand()%2 == 0);
	}

	// Aquí se actualizan las informaciones en el caso de que el agente pase en una casilla de zapatillas o de bikini
	if(sensores.terreno[0] == 'D' and !zapatillas){
		zapatillas = true;
	} else if(sensores.terreno[0] == 'K' and !bikini){
		bikini = true;
	}

	sensores.bateria = gastosBateria(accion, sensores);

	// Recordar la ultima accion
	last_action = accion;
	return accion;
}

void ComportamientoJugador::PonerTerrenoEnMatriz(const vector<unsigned char> &terreno, const state &st, vector< vector< unsigned char> > &matriz){
	// Extiende esta version inicial donde solo se pone la componente 0 en matriz
	// a poner todas las componentes de terreno en función de la orientación del agente.


	// En esta función se amplia el campo de visión del agente de manera a que este descubra más del mapa, teniendo en cuenta el vector terreno
	// Dicho campo de visión cambia en función de la orientación del agente, siendo dispuesto de igual forma cuando este está orientado al norte, este, sur y oeste
	// y cambiando para otra forma cuando se orienta al noreste, sureste, suroeste y noroeste

	int fil = st.fil;
	int col = st.col;

		matriz[fil][col] = terreno[0];
		
			switch(current_state.brujula){
				case norte:
					// Casillas a delante 
					matriz[fil - 1][col] = terreno[2];
					matriz[fil - 2][col] = terreno[6];
					matriz[fil - 3][col] = terreno[12];

					// Casillas a la izquierda
					matriz[fil - 1][col - 1] = terreno[1];
					matriz[fil - 2][col - 1] = terreno[5];
					matriz[fil - 3][col - 1] = terreno[11];
					matriz[fil - 2][col - 2] = terreno[4];
					matriz[fil - 3][col - 2] = terreno[10];
					matriz[fil - 3][col - 3] = terreno[9];

					// Casillas a la derecha
					matriz[fil - 1][col + 1] = terreno[3];
					matriz[fil - 2][col + 1] = terreno[7];
					matriz[fil - 3][col + 1] = terreno[13];
					matriz[fil - 2][col + 2] = terreno[8];
					matriz[fil - 3][col + 2] = terreno[14];
					matriz[fil - 3][col + 3] = terreno[15];
					break;

				case noreste:
					matriz[fil - 1][col] = terreno[1];
					matriz[fil - 1][col + 1] = terreno[2];
					matriz[fil][col + 1] = terreno[3];
					matriz[fil - 2][col + 1] = terreno[4];
					matriz[fil - 2][col + 1] = terreno[5];
					matriz[fil - 2][col + 2] = terreno[6];
					matriz[fil - 1][col + 2] = terreno[7];
					matriz[fil][col + 2] = terreno[8];
					matriz[fil - 3][col] = terreno[9];
					matriz[fil - 3][col + 1] = terreno[10];
					matriz[fil - 3][col + 2] = terreno[11];
					matriz[fil - 3][col + 3] = terreno[12];
					matriz[fil - 2][col + 3] = terreno[13];
					matriz[fil - 1][col + 3] = terreno[14];
					matriz[fil][col + 3] = terreno[15];
					break;

				case este:
					// Casillas a delante 
					matriz[fil][col + 1] = terreno[2];
					matriz[fil][col + 2] = terreno[6];
					matriz[fil][col + 3] = terreno[12];

					// Casillas a la izquierda
					matriz[fil - 1][col + 1] = terreno[1];
					matriz[fil - 1][col + 2] = terreno[5];
					matriz[fil - 1][col + 3] = terreno[11];
					matriz[fil - 2][col + 2] = terreno[4];
					matriz[fil - 2][col + 3] = terreno[10];
					matriz[fil - 3][col + 3] = terreno[9];

					// Casillas a la derecha
					matriz[fil + 1][col + 1] = terreno[3];
					matriz[fil + 1][col + 2] = terreno[7];
					matriz[fil + 1][col + 3] = terreno[13];
					matriz[fil + 2][col + 2] = terreno[8];
					matriz[fil + 2][col + 3] = terreno[14];
					matriz[fil + 3][col + 3] = terreno[15];
					break;

				case sureste:
					matriz[fil][col + 1] = terreno[1];
					matriz[fil + 1][col + 1] = terreno[2];
					matriz[fil + 1][col] = terreno[3];
					matriz[fil][col + 2] = terreno[4];
					matriz[fil + 1][col + 2] = terreno[5];
					matriz[fil + 2][col + 2] = terreno[6];
					matriz[fil + 2][col + 1] = terreno[7];
					matriz[fil + 2][col] = terreno[8];
					matriz[fil][col + 3] = terreno[9];
					matriz[fil + 1][col + 3] = terreno[10];
					matriz[fil + 2][col + 3] = terreno[11];
					matriz[fil + 3][col + 3] = terreno[12];
					matriz[fil + 3][col + 2] = terreno[13];
					matriz[fil + 3][col + 1] = terreno[14];
					matriz[fil + 3][col] = terreno[15];
					break;

				case sur:
					// Casillas a delante 
					matriz[fil + 1][col] = terreno[2];
					matriz[fil + 2][col] = terreno[6];
					matriz[fil + 3][col] = terreno[12];

					// Casillas a la izquierda
					matriz[fil + 1][col + 1] = terreno[1];
					matriz[fil + 2][col + 1] = terreno[5];
					matriz[fil + 3][col + 1] = terreno[11];
					matriz[fil + 2][col + 2] = terreno[4];
					matriz[fil + 3][col + 2] = terreno[10];
					matriz[fil + 3][col + 3] = terreno[9];

					// Casillas a la derecha
					matriz[fil + 1][col - 1] = terreno[3];
					matriz[fil + 2][col - 1] = terreno[7];
					matriz[fil + 3][col - 1] = terreno[13];
					matriz[fil + 2][col - 2] = terreno[8];
					matriz[fil + 3][col - 2] = terreno[14];
					matriz[fil + 3][col - 3] = terreno[15];
					break;

				case suroeste:
					matriz[fil + 1][col] = terreno[1];
					matriz[fil + 1][col - 1] = terreno[2];
					matriz[fil][col - 1] = terreno[3];
					matriz[fil + 2][col] = terreno[4];
					matriz[fil + 2][col - 1] = terreno[5];
					matriz[fil + 2][col - 2] = terreno[6];
					matriz[fil + 1][col - 2] = terreno[7];
					matriz[fil][col - 2] = terreno[8];
					matriz[fil + 3][col] = terreno[9];
					matriz[fil + 3][col - 1] = terreno[10];
					matriz[fil + 3][col - 2] = terreno[11];
					matriz[fil + 3][col - 3] = terreno[12];
					matriz[fil + 2][col - 3] = terreno[13];
					matriz[fil + 1][col - 3] = terreno[14];
					matriz[fil][col - 3] = terreno[15];
					break;

				case oeste:
					// Casillas a delante 
					matriz[fil][col - 1] = terreno[2];
					matriz[fil][col - 2] = terreno[6];
					matriz[fil][col - 3] = terreno[12];

					// Casillas a la izquierda
					matriz[fil + 1][col - 1] = terreno[1];
					matriz[fil + 1][col - 2] = terreno[5];
					matriz[fil + 1][col - 3] = terreno[11];
					matriz[fil + 2][col - 2] = terreno[4];
					matriz[fil + 2][col - 3] = terreno[10];
					matriz[fil + 3][col - 3] = terreno[9];

					// Casillas a la derecha
					matriz[fil - 1][col - 1] = terreno[3];
					matriz[fil - 1][col - 2] = terreno[7];
					matriz[fil - 1][col - 3] = terreno[13];
					matriz[fil - 2][col - 2] = terreno[8];
					matriz[fil - 2][col - 3] = terreno[14];
					matriz[fil - 3][col - 3] = terreno[15];
					break;

				case noroeste:
					matriz[fil][col - 1] = terreno[1];
					matriz[fil - 1][col - 1] = terreno[2];
					matriz[fil - 1][col] = terreno[3];
					matriz[fil][col - 2] = terreno[4];
					matriz[fil - 1][col - 2] = terreno[5];
					matriz[fil - 2][col - 2] = terreno[6];
					matriz[fil - 2][col - 1] = terreno[7];
					matriz[fil - 2][col] = terreno[8];
					matriz[fil][col - 3] = terreno[9];
					matriz[fil - 1][col - 3] = terreno[10];
					matriz[fil - 2][col - 3] = terreno[11];
					matriz[fil - 3][col - 3] = terreno[12];
					matriz[fil - 3][col - 2] = terreno[13];
					matriz[fil - 3][col - 1] = terreno[14];
					matriz[fil - 3][col] = terreno[15];
					break;
				}
		}


// Función para verificar si hay algún obstaculo
bool ComportamientoJugador::esObstaculo(unsigned char casilla){
	if(casilla == 'P' or casilla == 'M'){
		return true;
	} else {
		return false;
	}
}

// Verifica si la casilla a delante es un obstaculo
bool ComportamientoJugador::hayObstaculoDelante(state &st){
	int fil = st.fil;
	int col = st.col;

	switch (st.brujula){
	
	case norte:
		fil--;
		break;
	
	case noreste:
		fil--;
		col++;
		break;
	
	case este:
		col++;
		break;

	case sureste:
		fil++;
		col++;
		break;

	case sur:
		fil++;
		break;

	case suroeste:
		fil++;
		col--;
		break;

	case oeste:
		col--;
		break;

	case noroeste:
		fil--;
		col--;
		break;
	}

	// Verificación de los rangos del mapa
	if (fil < 0 or fil >= mapaResultado.size()){
		return true;
	}
	if(col < 0 or col >= mapaResultado[0].size()){
		return true;
	}

	// Comprobar el tipo de obstaculo
	if(!esObstaculo(mapaResultado[fil][col])){
		st.fil = fil;
		st.col = col;
		return false;
	} else {
		return true;
	}
}

int ComportamientoJugador::gastosBateria(Action accion, Sensores sensores){
	int bateria = sensores.bateria;
	switch(accion){
		case actFORWARD:
			switch(sensores.terreno[2]){
				case 'A':
					if(bikini){
						bateria -= 10;
					} else {
						bateria -= 100;
					}
					break;
				case 'B':
					if(zapatillas){
						bateria -= 15;
					} else {
						bateria -= 100;
					}
					break;
				case 'T':
					bateria -= 2;
					break;
				default:
					bateria -= 1;
					break;
			}
			break;
		case actTURN_SL or actTURN_SR:
			switch(sensores.terreno[2]){
				case 'A':
					if(bikini){
						bateria -= 5;
					} else {
						bateria -= 500;
					}
					break;
				case 'B':
					if(zapatillas){
						bateria -= 1;
					} else {
						bateria -= 3;
					}
					break;
				case 'T':
					bateria -= 2;
					break;
				default:
					bateria -= 1;
					break;
			}
			break;
		case actTURN_BL: // Aquí he intentado de hacer como en la línea 432 per me estaba dando un fallo deciendo que 'actTURN_BL ya estaba representado en la línea 432 
			switch(sensores.terreno[2]){
					case 'A':
						if(bikini){
							bateria -= 5;
						} else {
							bateria -= 50;
						}
						break;
					case 'B':
						if(zapatillas){
							bateria -= 1;
						} else {
							bateria -= 3;
						}
						break;
					case 'T':
						bateria -= 2;
						break;
					default:
						bateria -= 1;
						break;
			}
			break;
			case actTURN_BR:
				switch(sensores.terreno[2]){
						case 'A':
							if(bikini){
								bateria -= 5;
							} else {
								bateria -= 50;
							}
							break;
						case 'B':
							if(zapatillas){
								bateria -= 1;
							} else {
								bateria -= 3;
							}
							break;
						case 'T':
							bateria -= 2;
							break;
						default:
							bateria -= 1;
							break;
				}
				break;
	}
	return bateria;
}

// La función reset pondrá todos los valores a los iniciales
void ComportamientoJugador::reset(){
	current_state.brujula = norte;
	current_state.col = current_state.fil = 99;
	girar_derecha = false;
	bien_situado = false;
	zapatillas = bikini = false;
	last_action = actIDLE;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
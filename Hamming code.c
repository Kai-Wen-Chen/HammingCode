#include <stdio.h>
#include <stdlib.h>
#include <time.h>
char data[161];
char encoded[169];
char defect[169];
char decoded[161];

int main() {
	srand(time(NULL)); //initial random seed
	int i;
	int databits[160] = {0};
	FILE *output;
	
	output = fopen("output.txt", "w"); //create output.txt and write output in it
/*----Generate the original data------------------------*/	
	for (i=0; i<160; i++) databits[i] = rand() % 2;
	for (i=0; i<160; i++) data[i] = databits[i] + 48; //generate data bits
	databits[160] = '\0';
	
	//printf("original data: ");
	//for (i=0; i<20; i++) printf("%c", data[i]);
	//printf("\n");
	
	fputs("original data: ", output);
	fputs(data, output); //write original data
	fputc('\n', output);
/*----Encode the original data--------------------------*/
	int bit1_list [160][8];
	int checkbits[8] = {0};
	int count = 0;
	int j = 0;
	
	for (i=0; i<160; i++) { //find the bit-1 place
		if (databits[i] == 1) {
			int I = 160-i;
			int module = 128;
			
			for (j=0; j<8; j++) {
				if (I >= module) {	
					bit1_list[count][j] = 1;
					I -= module;
				}
				
				else bit1_list[count][j] = 0;
				
				module /= 2;	
			}
			
			for (j=0; j<8; j++) printf("%d", bit1_list[count][j]);
			printf("\n");
			count++;
		}
	}
	
	for (i=0; i<8; i++) { //XOR to generate check bits
		for (j=0; j<count; j++) {
			if (checkbits[i] != bit1_list[j][i]) checkbits[i] = 1;
			else checkbits[i] = 0;
		}
	}
	
	printf("check bits: ");
	for (i=0; i<8; i++) printf("%d", checkbits[i]);
	printf("\n");
	
	j = 0;
	int k = 0;
	
	for (i=0; i<168; i++) { //put check bits
		switch(i) {
			case 167: //1st
				encoded[i] = checkbits[k] + 48;
				k++;
				break;
			case 166: //2nd
				encoded[i] = checkbits[k] + 48;
				k++;
				break;
			case 164: //4th
				encoded[i] = checkbits[k] + 48;
				k++;
				break;
			case 160: //8th
				encoded[i] = checkbits[k] + 48;
				k++;
				break;
			case 152: //16th
				encoded[i] = checkbits[k] + 48;
				k++;
				break;
			case 136: //32nd
				encoded[i] = checkbits[k] + 48;
				k++;
				break;
			case 104: //64th
				encoded[i] = checkbits[k] + 48;
				k++;
				break;
			case 40: //128th
				encoded[i] = checkbits[k] + 48;
				k++;
				break;
			default:
				encoded[i] = databits[j] + 48;
				j++;
				break;
		}
	}
	encoded[168] = '\0';
	fputs("encoded data: ", output);
	fputs(encoded, output);
	fputc('\n', output);
/*----Random ruin a bit of encoded data-----------------*/
	int ruinbit;
	
	while (ruinbit = rand() % 169){
		if (ruinbit != 167 && ruinbit != 166 && ruinbit != 164
			&& ruinbit != 160 && ruinbit != 152 && ruinbit != 136
			&& ruinbit != 104 && ruinbit != 40) break;
	}
	
	for (i=0; i<168; i++) defect[i] = encoded[i];
	
	if (ruinbit != 168){
		if (defect[ruinbit] == '0') defect[ruinbit] = '1';
		else defect[ruinbit] = '0';
		
		printf("%d\n", 168-ruinbit);
	}
	
	defect[168] = '\0';
	
	//printf("defect: ");
	//for (i=0; i<25; i++) printf("%c", defect[i]);
	//printf("\n");
	fputs("defect data:  ", output);
	fputs(defect, output);
	fputc('\n', output);
/*----Decode the encoded data---------------------------*/
	int r_checkbits[8] = {defect[40] - 48, defect[104] - 48,
	 defect[136] - 48, defect[152] - 48, defect[160] - 48,
	 defect[164] - 48, defect[166] - 48, defect[167] - 48};
	int r_bit1_list[160][8];
	int decodedbits[160];
	j = 0;
	count = 0;
	
	printf("received check: ");
	for (i=0; i<8; i++) printf("%d", r_checkbits[i]);
	printf("\n");
	
	for (i=0; i<168; i++) { //take out the data code
		if (i == 40 || i == 104 || i == 136 || i == 152 || i == 160
			|| i == 164 || i == 166 || i == 167) continue;
		else {
			decodedbits[j] = defect[i] - 48;
			j++;
		}
	}
	
	//printf("decoded bits: ");
	//for (i=0; i<20; i++) printf("%d", decodedbits[i]);
	//printf("\n");
	
	for (i=0; i<160; i++) { //find the bit-1 place
		if (decodedbits[i] == 1) {
			int I = 160-i;
			int module = 128;
			
			for (j=0; j<8; j++) {
				if (I >= module) {	
					r_bit1_list[count][j] = 1;
					I -= module;
				}
				
				else r_bit1_list[count][j] = 0;
				
				module /= 2;	
			}
			
			for (j=0; j<8; j++) printf("%d", r_bit1_list[count][j]);
			printf("\n");
			count++;
		}
	}
	
	for (i=0; i<160; i++) decoded[i] = decodedbits[i] + 48;
	decoded[160] = '\0';
	
	for (i=0; i<8; i++) { //XOR to generate check bits
		for (j=0; j<count; j++) {
			if (r_checkbits[i] != r_bit1_list[j][i]) r_checkbits[i] = 1;
			else r_checkbits[i] = 0;
		}
	}
	
	printf("after checked: ");
	for (i=0; i<8; i++) printf("%d", r_checkbits[i]);
	printf("\n");
	
	fputs("decoded data:  ", output);
	fputs(decoded, output);
	fputc('\n', output);
/*----Check the decoded data----------------------------*/
	int place=0;
	int exp = 128;
		
	for (i=0; i<8; i++) {
		place += r_checkbits[i] * exp;
		exp /= 2;
	}
	
	fputs("comparing result: ", output);
	if (place == 0) fputs("corrected", output);
	else {
		char plc[4];
		
		if (place / 100 > 0){
			plc[0] = place/100 + 48; //hundred
			plc[1] = (place%100)/10 + 48; //ten
			plc[2] = place%10 + 48; //unit
			plc[3] = '\0';
		} else if (place / 10 > 0) {
			plc[0] = place/10 + 48; //ten
			plc[1] = place%10 + 48; //unit
			plc[2] = '\0';
		} else{
			plc[0] = place + 48;
			plc[1] = '\0';
		}
		fputs("not corrected, ", output);
		fputs("No.", output);
		fputs(plc, output);
		fputs(" bit is wrong", output);
	}
/*----Finish--------------------------------------------*/	
	fclose(output);
	
	return 0;
}

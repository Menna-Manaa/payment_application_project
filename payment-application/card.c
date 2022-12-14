#ifndef _CARD_C_
#define _CARD_C_
#include "card.h"
#include <random>
#include <time.h>
#include <stdio.h>
int getNeededLuhnDigit(uint8_t* cardPan) {
	int startindex = 17;
	int i;
	int sum = 0;
	char cp[21];
	for (i = 0; i <= startindex; i++) {
		cp[i] = cardPan[i];
	}
	cp[startindex + 1] = '\0';
	for (i = startindex; i >= 0; i -= 2) {
		int digit = (cp[i] - '0') * 2;
		if (digit < 10) {
			cp[i] = (char)(digit + '0');
		}
		else {
			cp[i] = (char)(digit - 9 + '0');
		}
	}
	for (i = 0; i <= startindex; i++) {
		sum += cp[i] - '0';
	}
	return sum % 10;
}

uint8_t* GenerateLuhn() {
	time_t t1;
	int i;
	srand((unsigned)time(&t1));
	uint8_t* cardpan;
	cardpan[0] = (char)('0' + rand() % 9 + 1);
	for (i = 1; i <= 17; i++) {
		cardpan[i] = (char)('0' + rand() % 10);
	}
	cardpan[18] = '\0';
	int valid = getNeededLuhnDigit(cardpan);
	cardpan[18] = (char)('0' + valid);
	cardpan[19] = '\0';
	return cardpan;
}


EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	char name[100];
	int i;
	for (i = 0; i < 100; i++) {
		name[i] = '\0';
	}
	printf("Enter card holder name [20-24 characters]: ");
	fseek(stdin, 0, SEEK_END);
	int i = 0;
	char ch;
	while ((ch = getchar()) != '\n') {
		name[i] = ch;
		i++;
	}
	fseek(stdin, 0, SEEK_END);
	if (!name) {
		return WRONG_NAME;
	}
	if (name[18] == '\0' || name[24] != '\0') {
		return WRONG_NAME;
	}
	for (i = 0; i < 25; i++) {
		cardData->cardHolderName[i] = name[i];
	}
	printf("Read name: %s\n", cardData->cardHolderName);
	return (EN_cardError_t)OK_cardError;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	char cardexp[100];
	int i;
	for (i = 0; i < 100; i++)cardexp[i] = '\0';
	uint32_t month, year;
	printf("Enter expiry date [MM/YY]: ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", cardexp);
	if (!cardexp) {
		return WRONG_EXP_DATE;
	}
	if (cardexp[6] != '\0' || cardexp[3] == '\0') {
		return WRONG_EXP_DATE;
	}
	month = (cardexp[0] - '0') * 10 + (cardexp[1] - '0');
	year = (cardexp[3] - '0') * 10 + (cardexp[4] - '0');
	if (month < 1 || month>12 || cardexp[2] != '/' || (year > 99 || year < 0)) {
		return WRONG_EXP_DATE;
	}
	for (i = 0; i < 6; i++) {
		cardData->cardExpirationDate[i] = cardexp[i];
	}
	printf("Read expiry date: %s\n", cardData->cardExpirationDate);
	return (EN_cardError_t)OK_cardError;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	char ans = '\0';
	printf("Do you want to generate Luhn number? [y/n]\n");
	fseek(stdin, 0, SEEK_END);
	scanf("%c", &ans);
	if (ans == 'y' || ans == 'Y') {
		uint8_t* cardpan = GenerateLuhn();
		for (int i = 0; i < 20; i++) {
			cardData->primaryAccountNumber[i] = cardpan[i];
		}
		printf("Generated card number: %s\n", cardData->primaryAccountNumber);
	}
	else {
		char cardpan[100];
		int i;
		for (i = 0; i < 100; i++)cardpan[i] = '\0';
		printf("Enter PAN [16-19 characters]:");
		int i = 0;
		char ch;
		fseek(stdin, 0, SEEK_END);
		while ((ch = getchar()) != '\n') {
			cardpan[i] = ch;
			i++;
		}
		if (!cardpan)return WRONG_PAN;
		if (cardpan[19] != '\0' || cardpan[14] == '\0')return WRONG_PAN;
		for (int i = 0; i < 20; i++) {
			cardData->primaryAccountNumber[i] = cardpan[i];
		}
		printf("Read PAN: %s\n", cardData->primaryAccountNumber);
	}
	return OK_cardError;
}
#endif
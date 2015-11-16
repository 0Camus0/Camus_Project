#include <Utils/Maths.h>


#if ATOF_TOM_VAN
#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')

double atof_tom(const char *p)
{
	int frac;
	double sign, value, scale;

	// Skip leading white space, if any.

	while (white_space(*p)) {
		p += 1;
	}

	// Get sign, if any.

	sign = 1.0;
	if (*p == '-') {
		sign = -1.0;
		p += 1;

	}
	else if (*p == '+') {
		p += 1;
	}

	// Get digits before decimal point or exponent, if any.

	for (value = 0.0; valid_digit(*p); p += 1) {
		value = value * 10.0 + (*p - '0');
	}

	// Get digits after decimal point, if any.

	if (*p == '.') {
		double pow10 = 10.0;
		p += 1;
		while (valid_digit(*p)) {
			value += (*p - '0') / pow10;
			pow10 *= 10.0;
			p += 1;
		}
	}

	// Handle exponent, if any.

	frac = 0;
	scale = 1.0;
	if ((*p == 'e') || (*p == 'E')) {
		unsigned int expon;

		// Get sign of exponent, if any.

		p += 1;
		if (*p == '-') {
			frac = 1;
			p += 1;

		}
		else if (*p == '+') {
			p += 1;
		}

		// Get digits of exponent, if any.

		for (expon = 0; valid_digit(*p); p += 1) {
			expon = expon * 10 + (*p - '0');
		}
		if (expon > 308) expon = 308;

		// Calculate scaling factor.

		while (expon >= 50) { scale *= 1E50; expon -= 50; }
		while (expon >= 8) { scale *= 1E8;  expon -= 8; }
		while (expon > 0) { scale *= 10.0; expon -= 1; }
	}

	// Return signed and scaled floating point result.

	return sign * (frac ? (value / scale) : (value * scale));
}
#endif

#if ATOF_ANGEL
#define  inicioNumeros 48
float CadenaAnumeroFlotantePersonalizada(char *c) {
	float numero = 0.0f;
	int index = 0;
	char *ptr = c;

	//Buscando si tiene notacion exponencial
	while (ptr[index] != '\0') {
		if (ptr[index] == 'e') {
			return numero;
		}
		index++;
	}

	index = 0;
	bool negativo = ptr[index] == '-';
	if (negativo) {
		index++;
	}

	int point = 0;
	int last_index = index;
	while (ptr[index] != '\0') {
		if (ptr[index] == '.') {
			point = index;
			break;
		}
		index++;
	}
	point--;

	float num = 0.0f;
	float multiplicador = 1.0f;

	int i = point;
	do {
		numero += (ptr[i] - inicioNumeros)*multiplicador;
		multiplicador *= 10.0f;
		i--;
	} while (i >= last_index && i >= 0);


	index = last_index;

	if (ptr[index] != '\0') {
		while (ptr[index] != '\0') {
			index++;
		}
		multiplicador = 1.0f;
		for (i = index; i > (point + 2); i--) {
			num += (ptr[i - 1] - inicioNumeros)*multiplicador;
			multiplicador *= 10.0f;
		}
		num /= multiplicador;
		numero += num;
	}

	if (negativo) {
		numero = -numero;
	}

	return numero;
}
#endif
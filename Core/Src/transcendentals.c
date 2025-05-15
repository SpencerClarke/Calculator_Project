/* Computes f^i */
double powfi(double f, int i)
{
	int j;
	int negative = 0;
	double out = 1.0;

	if(i < 0)
	{
		negative = 1;
		i = -i;
	}

	for(j = 0; j < i; j++)
	{
		out *= f;
	}

	if(negative)
	{
		return 1.0/out;
	}
	else
	{
		return out;
	}
}

double absf(double f)
{
	if(f < 0)
		f = -f;
	return f;
}

/* Approximates ln(f) where f is close to 1 using Taylor Series */
double lnf_near_1(double f)
{
    double out;
    int negative = 0;
	/* ln_LUT[] = {ln(1.0), ln(1.1), ln(1.2) ... ln(2.0)} */
	double ln_LUT[] = {0.0, 0.09531017980432493, 0.1823215567939546, 0.26236426446749106, 0.3364722366212129, 0.4054651081081644, 0.47000362924573563, 0.5306282510621704, 0.5877866649021191, 0.6418538861723947, 0.6931471805599453};
	double min_diff = 100.0;
	double expansion_point = 1.0;
	double expansion_point_value = 0.0;
	double divisor;
	int i;
	for(i = 0; i < 10; i++)
	{
		if(min_diff > absf(f - (1 + i/10.0)))
		{
			min_diff = absf(f - (1 + i/10.0));
			expansion_point = 1 + 0.1 * i;
			expansion_point_value = ln_LUT[i];
		}
	}
	
	out = expansion_point_value;
	divisor = expansion_point;
	for(i = 1; i < 30; i++)
	{
		if(negative)
		{
			out -= powfi(f-expansion_point, i) / i / divisor;
			negative = 0;
		}
		else
		{
			out += powfi(f-expansion_point, i) / i / divisor;
			negative = 1;
		}
		divisor *= expansion_point;
	}

	return out;
}

/* Computes ln(f) */
double lnf(double f)
{
        double out;
        long long exponent = (long long)(((*((unsigned long long *)&f) & 0x7ff0000000000000) >> 52) - 1023);
        unsigned long long mantissa = *((unsigned long long *)&f) & 0xfffffffffffff;

        /* Now we'll get the numerical value of the mantissa 1.XXXXX... */
        double fraction;

        /* Set the exponent to 1 (1023 after applying bias */
        *((unsigned long long *)&fraction) = ((unsigned long long)1023 << 52);

        /* Insert the mantissa */
        *((unsigned long long *)&fraction) |= mantissa;

        /* f = fraction * 2^exponent */
        /* ln(f) = ln(fraction * 2^exponent) */
        /* ln(f) = ln(fraction) + ln(2^exponent) */
        /* ln(f) = ln(fraction) + exponent * ln(2) */
        /* ... and fraction is close to 1 */
        out = lnf_near_1(fraction) + exponent * 0.6931471806;

        return out;
}
/* computes f1^f2 where f2 is close to 0 */
double powff_near_0(double f1, double f2)
{
	double out = 1.0;
	double ln_f1 = lnf(f1);
	double multiplier = 1.0;
	double factorial_divisor = 1;
	for(int i = 1; i < 20; i++)
	{
		multiplier *= ln_f1;
		factorial_divisor *= i;
		out += powfi(f2, i) * multiplier / factorial_divisor;
	}
	
	return out;
}
/* Computes f1^f2 */
double powff(double f1, double f2)
{
	int whole_component = (int)f2;
	double decimal_component = f2 - whole_component;

	return powfi(f1, whole_component) * powff_near_0(f1, decimal_component);
}
double sin_near_0(double f)
{
        double out = f;
        double factorial_divisor = 1;
        int i;
        int is_negative = 1;

        /* sin(f) = f - f^3/3! + f^5/5! - f^7/7! ... */
        for(i = 3; i < 20; i+=2)
        {
                factorial_divisor *= i;
                factorial_divisor *= (i - 1);
                if(is_negative)
                {
                        out -= powfi(f, i) / factorial_divisor;
                }
                else
                {
                        out += powfi(f, i) / factorial_divisor;
                }
                is_negative = !is_negative;
        }
        return out;
}
double cos_near_0(double f)
{
        double out = 1;
        int is_negative = 1;
        double factorial_divisor = 1;
        int i;

        /* cos(f) = 1 - f^2/2! + f^4/4! - f^6/6! + ... */
        for(i = 2; i < 22; i += 2)
        {
                factorial_divisor *= i;
                factorial_divisor *= (i - 1);
                if(is_negative)
                {
                        out -= powfi(f, i) / factorial_divisor;
                }
                else
                {
                        out += powfi(f, i) / factorial_divisor;
                }
                is_negative = !is_negative;
        }
        return out;
}

double cosine(double f)
{
        f /= (3.1415926535897932384626 * 2);
        /* Now we round down to the nearest integer (or nearest multiple of 2pi when not scaled) */
        long long integer_part = (long long)f;
        if(f < 0)
        {
                integer_part -= 1; /* Round down instead of round towards 0 */
        }
        double remainder = f - integer_part; /* Get its position relative to the nearest integer (i.e. 2pi) */
        remainder *= (3.1415926535897932384626 * 2); /* Scale back up */
        return cos_near_0(remainder);
}
double sine(double f)
{
        f /= (3.1415926535897932384626 * 2);
        /* Now we round down to the nearest integer (or nearest multiple of 2pi when not scaled) */
        long long integer_part = (long long)f;
        if(f < 0)
        {
                integer_part -= 1; /* Round down instead of round towards 0 */
        }
        double remainder = f - integer_part; /* Get its position relative to the nearest integer (i.e. 2pi) */
        remainder *= (3.1415926535897932384626 * 2); /* Scale back up */
        return sin_near_0(remainder);
}

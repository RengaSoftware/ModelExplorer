#include "qtpropertycommon.h"

QString formatDoubleToString(const double value, const bool allowScientific, const uint precision)
{
	QString number = QString::number(value, allowScientific ? 'g' : 'f', precision); 
	const bool isFractional = (abs(value - floor(value)) > 0);

	if (isFractional)
	{
		while (number.at(number.length() - 1) == QChar('0'))
			number.remove(number.length() - 1, 1);

        if (number.at(number.length() - 1) == QChar('.'))
            number.remove(number.length() - 1, 1);
	}
	else
	{
		number = QString::number(value, allowScientific ? 'g' : 'f', allowScientific ? precision : 0);
	}
    
    return number;
}
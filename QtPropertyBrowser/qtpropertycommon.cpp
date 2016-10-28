#include "qtpropertycommon.h"

bool formatDoubleString(QString& inputString)
{
	bool ok = false;
	double doubleValue = inputString.toDouble(&ok);
	if (ok && inputString.length() > 0)
	{
		if (abs(doubleValue) > max_value)
			doubleValue = (doubleValue < 0 ? min_value : max_value);

		inputString = QString::number(doubleValue, 'f', 3); 
		const bool isFractional = (abs(doubleValue - floor(doubleValue)) > 0);

		if (isFractional)
		{
			while (inputString.at(inputString.length() - 1) == QChar('0'))
				inputString.remove(inputString.length() - 1, 1);
		}
		else
		{
			inputString = QString::number(doubleValue, 'f', 0);
		}
	}
	return ok;
}
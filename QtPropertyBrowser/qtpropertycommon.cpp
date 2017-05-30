#include "qtpropertycommon.h"
#include <QtCore/QStringList.h>
#include <QtCore/QLocale.h>

#include <assert.h>

QString formatDoubleToString(const double value, const bool allowScientific, const uint precision)
{
  QString number = QLocale::system().toString(value, allowScientific ? 'g' : 'f', precision);  
  QStringList numberParts = number.split(QLocale::system().decimalPoint());
  if (numberParts.size() == 1)
  {
    return number;
  }
  else if (numberParts.size() == 2)
  {
    QString fractionalPart = numberParts.back();
    
    while (fractionalPart.size() && fractionalPart.lastIndexOf(QChar('0')) == fractionalPart.size() - 1)
      fractionalPart = fractionalPart.remove(fractionalPart.size() - 1, 1);
    
    if (fractionalPart.isEmpty())
      return numberParts.front();
    else
      return numberParts.front() + QLocale::system().decimalPoint() + fractionalPart;
  }
  else
  {
    assert(false);
  }
}

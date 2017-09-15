//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS.
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "BoolGuard.h"

BoolGuard::BoolGuard(bool& variable)
  : m_variable(variable)
{
}

BoolGuard::BoolGuard(bool& variable, bool initialValue)
  : m_variable(variable)
{
  m_variable = initialValue;
}

BoolGuard::~BoolGuard()
{
  m_variable = !m_variable;
}

//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

class BoolGuard
{
public:
  explicit BoolGuard(bool& variable);
  explicit BoolGuard(bool& variable, bool initialValue);

  BoolGuard(const BoolGuard& guard) = delete;
  BoolGuard& operator = (const BoolGuard& guard) = delete;

  ~BoolGuard();

private:
  bool& m_variable;
};

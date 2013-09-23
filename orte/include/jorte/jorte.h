/*  jorte.h */

/**
  * @author Lukas Pokorny (lukas_pokorny@centrum.cz)
  * @author CTU FEE Prague - Department of Control Engineering (dce.felk.cvut.cz)
  * @author Project ORTE - OCERA Real Time Ethernet (www.ocera.org)
  * @author dedication to Kj
  * @version 0.1
  *
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  * GNU General Public License for more details.
  *
  */

/*
#ifndef _JORTE_H
#define _JORTE_H
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "orte.h"

// switch program behaviour (test stage,....)
#include "4all.h"

// NtpTime
#include "getNtpTime.h"

// new type definitions
#include "typedefs_defines_jorte.h"

// auto-generated headers by JNI
#include "org_ocera_orte_tools_Conversions.h"
#include "org_ocera_orte_Domain.h"
#include "org_ocera_orte_DomainEvents.h"
#include "org_ocera_orte_DomainProp.h"
#include "org_ocera_orte_tools_GetORTEConstant.h"
#include "org_ocera_orte_JOrte.h"
#include "org_ocera_orte_Manager.h"
#include "org_ocera_orte_Publication.h"
#include "org_ocera_orte_Subscription.h"

#ifdef __cplusplus
} /* extern "C"*/
#endif

/*
#endif // _JORTE_H
*/

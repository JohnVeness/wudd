/****************************************************************************
 * Copyright (C) 2016-2021 Maschell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#pragma once

#include <cstdint>
#include <WUD/entities/TMD/Content.h>
#include <WUD/entities/FST/FST.h>

class NUSDataProvider {

public:
    virtual ~NUSDataProvider() = default;

    virtual bool readRawContent(Content *content, uint8_t *buffer, uint64_t offset, uint32_t size) = 0;

    virtual bool getContentH3Hash(Content *content, uint8_t **data, uint32_t *size) = 0;

    virtual bool getRawTMD(uint8_t **data, uint32_t *size) = 0;

    virtual bool getRawTicket(uint8_t **data, uint32_t *size) = 0;

    virtual bool getRawCert(uint8_t **data, uint32_t *size) = 0;

    virtual void setFST(FST *fst) {

    }
};
/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

#include <array/ArrayOptions.h>
#include <helpers/shape.h>
#include <stdexcept>

namespace nd4j {
    bool ArrayOptions::isNewFormat(Nd4jLong *shapeInfo) {
        return (shape::extra(shapeInfo) != 0);
    }


    bool ArrayOptions::isSparseArray(Nd4jLong *shapeInfo) {
        return hasPropertyBitSet(shapeInfo, ARRAY_SPARSE);
    }

    bool ArrayOptions::hasExtraProperties(Nd4jLong *shapeInfo) {
        return hasPropertyBitSet(shapeInfo, ARRAY_EXTRAS);
    }

    bool ArrayOptions::hasPropertyBitSet(Nd4jLong *shapeInfo, int property) {
        if (!isNewFormat(shapeInfo))
            return false;

        return ((shape::extra(shapeInfo) & property) == property);
    }

    bool ArrayOptions::isUnsigned(Nd4jLong *shapeInfo) {
        if (!isNewFormat(shapeInfo))
            return false;

        return hasPropertyBitSet(shapeInfo, ARRAY_UNSIGNED);
    }

    nd4j::DataType ArrayOptions::dataType(const Nd4jLong *shapeInfo) {
        return dataType(const_cast<Nd4jLong *>(shapeInfo));
    }

    nd4j::DataType ArrayOptions::dataType(Nd4jLong *shapeInfo) {
        if (hasPropertyBitSet(shapeInfo, ARRAY_QUANTIZED))
            return nd4j::DataType::DataType_QINT8;
        else if (hasPropertyBitSet(shapeInfo, ARRAY_FLOAT))
            return nd4j::DataType::DataType_FLOAT;
        else if (hasPropertyBitSet(shapeInfo, ARRAY_DOUBLE))
            return nd4j::DataType::DataType_DOUBLE;
        else if (hasPropertyBitSet(shapeInfo, ARRAY_HALF))
            return nd4j::DataType::DataType_HALF;
        else if (hasPropertyBitSet(shapeInfo, ARRAY_BOOL))
            return nd4j::DataType ::DataType_BOOL;
        else if (hasPropertyBitSet(shapeInfo, ARRAY_UNSIGNED)) {
            if (hasPropertyBitSet(shapeInfo, ARRAY_CHAR))
                return nd4j::DataType ::DataType_UINT8;
            else if (hasPropertyBitSet(shapeInfo, ARRAY_SHORT))
                return nd4j::DataType ::DataType_UINT16;
            else if (hasPropertyBitSet(shapeInfo, ARRAY_INT))
                return nd4j::DataType ::DataType_UINT32;
            else if (hasPropertyBitSet(shapeInfo, ARRAY_LONG))
                return nd4j::DataType ::DataType_UINT64;
            else
                throw std::runtime_error("Bad datatype");
        }
        else if (hasPropertyBitSet(shapeInfo, ARRAY_CHAR))
            return nd4j::DataType ::DataType_INT8;
        else if (hasPropertyBitSet(shapeInfo, ARRAY_SHORT))
            return nd4j::DataType ::DataType_INT16;
        else if (hasPropertyBitSet(shapeInfo, ARRAY_INT))
            return nd4j::DataType ::DataType_INT32;
        else if (hasPropertyBitSet(shapeInfo, ARRAY_LONG))
            return nd4j::DataType ::DataType_INT64;
        else
            throw std::runtime_error("Bad datatype");
    }

    SpaceType ArrayOptions::spaceType(const Nd4jLong *shapeInfo) {
        return spaceType(const_cast<Nd4jLong *>(shapeInfo));
    }

    SpaceType ArrayOptions::spaceType(Nd4jLong *shapeInfo) {
        if (hasPropertyBitSet(shapeInfo, ARRAY_QUANTIZED))
            return SpaceType::QUANTIZED;
        if (hasPropertyBitSet(shapeInfo, ARRAY_COMPLEX))
            return SpaceType::COMPLEX;
        else // by default we return continuous type here
            return SpaceType::CONTINUOUS;
    }

    ArrayType ArrayOptions::arrayType(const Nd4jLong *shapeInfo) {
        return arrayType(const_cast<Nd4jLong *>(shapeInfo));
    }

    ArrayType ArrayOptions::arrayType(Nd4jLong *shapeInfo) {
        if (hasPropertyBitSet(shapeInfo, ARRAY_SPARSE))
            return ArrayType::SPARSE;
        else if (hasPropertyBitSet(shapeInfo, ARRAY_COMPRESSED))
            return ArrayType::COMPRESSED;
        else if (hasPropertyBitSet(shapeInfo, ARRAY_EMPTY))
            return ArrayType::EMPTY;
        else // by default we return DENSE type here
            return ArrayType::DENSE;
    }

    bool ArrayOptions::togglePropertyBit(Nd4jLong *shapeInfo, int property) {
        shape::extra(shapeInfo) ^= property;

        return hasPropertyBitSet(shapeInfo, property);
    }

    void ArrayOptions::setPropertyBit(Nd4jLong *shapeInfo, int property) {
        shape::extra(shapeInfo) |= property;
    }

    void ArrayOptions::unsetPropertyBit(Nd4jLong *shapeInfo, int property) {
        shape::extra(shapeInfo) &= property;
    }

    SparseType ArrayOptions::sparseType(const Nd4jLong *shapeInfo) {
        spaceType(const_cast<Nd4jLong *>(shapeInfo));
    }

    SparseType ArrayOptions::sparseType(Nd4jLong *shapeInfo) {
        if (!isSparseArray(shapeInfo))
            throw std::runtime_error("Not a sparse array");

        if (hasPropertyBitSet(shapeInfo, ARRAY_CSC))
            return SparseType::CSC;
        else if (hasPropertyBitSet(shapeInfo, ARRAY_CSR))
            return SparseType::CSR;
        else if (hasPropertyBitSet(shapeInfo, ARRAY_COO))
            return SparseType::COO;
        else
            return SparseType::LIL;
    }

    void ArrayOptions::setPropertyBits(Nd4jLong *shapeInfo, std::initializer_list<int> properties) {
        for (auto v: properties) {
            if (!hasPropertyBitSet(shapeInfo, v))
                setPropertyBit(shapeInfo, v);
        }
    }

    void ArrayOptions::resetDataType(Nd4jLong *shapeInfo) {
        unsetPropertyBit(shapeInfo, ARRAY_BOOL);
        unsetPropertyBit(shapeInfo, ARRAY_HALF);
        unsetPropertyBit(shapeInfo, ARRAY_FLOAT);
        unsetPropertyBit(shapeInfo, ARRAY_DOUBLE);
        unsetPropertyBit(shapeInfo, ARRAY_INT);
        unsetPropertyBit(shapeInfo, ARRAY_LONG);
        unsetPropertyBit(shapeInfo, ARRAY_CHAR);
        unsetPropertyBit(shapeInfo, ARRAY_SHORT);
        unsetPropertyBit(shapeInfo, ARRAY_UNSIGNED);
    }

    void ArrayOptions::setDataType(Nd4jLong *shapeInfo, nd4j::DataType dataType) {
        resetDataType(shapeInfo);
        if (dataType == nd4j::DataType::DataType_UINT8 ||
                dataType == nd4j::DataType::DataType_UINT16 ||
                dataType == nd4j::DataType::DataType_UINT32 ||
                dataType == nd4j::DataType::DataType_UINT64) {

            setPropertyBit(shapeInfo, ARRAY_UNSIGNED);
        }

        if (dataType == nd4j::DataType::DataType_BOOL)
            setPropertyBit(shapeInfo, ARRAY_BOOL);
        else if (dataType == nd4j::DataType::DataType_HALF)
            setPropertyBit(shapeInfo, ARRAY_HALF);
        else if (dataType == nd4j::DataType::DataType_FLOAT)
            setPropertyBit(shapeInfo, ARRAY_FLOAT);
        else if (dataType == nd4j::DataType::DataType_DOUBLE)
            setPropertyBit(shapeInfo, ARRAY_DOUBLE);
        else if (dataType == nd4j::DataType::DataType_INT8)
            setPropertyBit(shapeInfo, ARRAY_CHAR);
        else if (dataType == nd4j::DataType::DataType_INT16)
            setPropertyBit(shapeInfo, ARRAY_SHORT);
        else if (dataType == nd4j::DataType::DataType_INT32)
            setPropertyBit(shapeInfo, ARRAY_INT);
        else if (dataType == nd4j::DataType::DataType_INT64)
            setPropertyBit(shapeInfo, ARRAY_LONG);
        else if (dataType == nd4j::DataType::DataType_UINT8)
            setPropertyBit(shapeInfo, ARRAY_CHAR);
        else if (dataType == nd4j::DataType::DataType_UINT16)
            setPropertyBit(shapeInfo, ARRAY_SHORT);
        else if (dataType == nd4j::DataType::DataType_UINT32)
            setPropertyBit(shapeInfo, ARRAY_INT);
        else if (dataType == nd4j::DataType::DataType_UINT64)
            setPropertyBit(shapeInfo, ARRAY_LONG);
        else
            throw std::runtime_error("Can't set unknown data type");
    }
}
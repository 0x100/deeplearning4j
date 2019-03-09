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

//
//  @author raver119@gmail.com
//

#include "../ConstantShapeHelper.h"
#include <logger.h>
#include <ShapeBuilders.h>
#include <ShapeUtils.h>

namespace nd4j {
    ConstantShapeHelper::ConstantShapeHelper() {
        _cache.resize(32);
        for (int e = 0; e < 32; e++) {
            std::map<ShapeDescriptor, DataBuffer> cache;
            _cache[e] = cache;
        }
    }

    ConstantShapeHelper* ConstantShapeHelper::getInstance() {
        if (!_INSTANCE)
            _INSTANCE = new ConstantShapeHelper();

        return _INSTANCE;
    }

    DataBuffer& ConstantShapeHelper::bufferForShapeInfo(nd4j::DataType dataType, char order, const std::vector<Nd4jLong> &shape) {
        ShapeDescriptor descriptor(dataType, order, shape);
        return bufferForShapeInfo(descriptor);
    }

    DataBuffer& ConstantShapeHelper::bufferForShapeInfo(ShapeDescriptor &descriptor) {
        int deviceId = 0;

        _mutex.lock();

        if (_cache[deviceId].count(descriptor) == 0) {
            auto hPtr = descriptor.toShapeInfo();
            DataBuffer buffer(hPtr, nullptr);
            ShapeDescriptor descriptor1(descriptor);
            _cache[deviceId][descriptor1] = buffer;
            DataBuffer &r = _cache[deviceId][descriptor1];
            _mutex.unlock();

            return r;
        } else {
            DataBuffer &r = _cache[deviceId].at(descriptor);
            _mutex.unlock();

            return r;
        }
    }

    DataBuffer& ConstantShapeHelper::bufferForShapeInfo(const Nd4jLong *shapeInfo) {
        ShapeDescriptor descriptor(shapeInfo);
        return bufferForShapeInfo(descriptor);
    }

    bool ConstantShapeHelper::checkBufferExistanceForShapeInfo(ShapeDescriptor &descriptor) {
        bool result;
        int deviceId = 0;
        _mutex.lock();

        if (_cache[deviceId].count(descriptor) == 0)
            result = false;
        else
            result = true;

        _mutex.unlock();

        return result;
    }

    nd4j::ConstantShapeHelper* nd4j::ConstantShapeHelper::_INSTANCE = 0;
}
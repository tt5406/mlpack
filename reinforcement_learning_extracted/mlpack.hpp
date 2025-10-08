/**
 * @file mlpack.hpp
 *
 * Convenience include for the reinforcement learning components of mlpack.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_HPP
#define MLPACK_HPP

// Include all of the core library components.
#include "mlpack/base.hpp"
#include "mlpack/prereqs.hpp"
#include "mlpack/core.hpp"

// Now include the reinforcement learning methods.
#include "mlpack/methods/reinforcement_learning.hpp"

// Include reverse compatibility.
#include "mlpack/namespace_compat.hpp"

#endif
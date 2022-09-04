
#ifndef INCLUDE_HAL_RNG_H_
#define INCLUDE_HAL_RNG_H_

#include "HalCommon.h"
namespace Hal
{

class Rng
{
public:
	Rng();
	~Rng();

	uint32_t GetNumber();

private:
	/// @brief	Hide Copy constructor.
	Rng(const Rng &) = delete;

	/// @brief	Hide Assignment operator.
	Rng &operator=(const Rng &) = delete;

	/// @brief	Hide Move constructor.
	Rng(Rng &&) = delete;

	/// @brief	Hide Move assignment operator.
	Rng &operator=(Rng &&) = delete;

};
} // namespace Hal

#endif /* INCLUDE_HAL_RNG_H_ */

#pragma once

#pragma warning(push, 0)
#include <random>
#pragma warning(pop)

#pragma warning(disable : 4003)
/**
 * \author Magdalena Majkowska
 * One of the most important class of our project - RandomNumberGenerator. 
 * Depending on the inital values given during the initialization it can generate 
 * either an integer or floating point random numbers from the linear distribution
 * and float/double values from normal distribution.
 * 
 */
template <typename Numeric, typename RealOnlyNumeric = float>
class RandomNumberGenerator
{
private:
    using distributionType = typename std::conditional<
        std::is_integral<Numeric>::value,
        std::uniform_int_distribution<Numeric>,
        std::uniform_real_distribution<Numeric>>::type;

    std::mt19937_64 generator_;
    distributionType distribution_;
    std::normal_distribution<RealOnlyNumeric> normalDistribution_;

public:
    /**
    * Constructor of this class. 
    * @param min - minimum value of the linear distribution
    * @param max - maximum value of the linear distribution
    * @param mean - mean parameter of the normal distribution
    * @param stddev - standard deviation parameter of the normal distribution
    */
    RandomNumberGenerator(Numeric min = 0.0f, Numeric max = 1.0f, RealOnlyNumeric mean = 5.0, RealOnlyNumeric stddev = 2.0)
    {
        std::random_device rd;
        this->generator_ = std::mt19937_64(rd());
        this->distribution_ = distributionType(min, max);
        this->normalDistribution_ = std::normal_distribution<RealOnlyNumeric>(mean, stddev);
    }
    /**
     * Method of getting a number from linear distribution
     *\return number from linear distribution with the parameters specified in the constructor 
     */
    Numeric get()
    {
        return distribution_(generator_);
    }
    /**
     * Method of getting a number from linear distribution and changing initial parameters.
     * \param min - new minimum value for the linear distribution
     * \param max - new maximum value for the linear distribution
     *\return number from linear distribution with new parameters
     */
    Numeric get(Numeric min, Numeric max)
    {
        typename distributionType::param_type d2(min, max);
        distribution_.param(d2);
        return distribution_(generator_);
    }
    /**
     * Method of getting a number from normal distribution
     *\return number from normal distribution with the parameters specified in the constructor 
     */
    RealOnlyNumeric getNormal()
    {
        return normalDistribution_(generator_);
    }
    /**
     * Method of getting a number from normal distribution and changing initial parameters.
     * \param mean - new mean parameter for normal distribution
     * \param stddev - new standard deviation parameter for normal distribution
     *\return number from normal distribution with new parameters
     */
    RealOnlyNumeric getNormal(RealOnlyNumeric mean, RealOnlyNumeric stddev)
    {
        this->normalDistribution_ = std::normal_distribution<RealOnlyNumeric>(mean, stddev);
        return normalDistribution_(generator_);
    }
    /**
     * Method of getting the smallest number of normal distribution.
     * For testing purposes only.
     * \return minimum possible number that we will get from normal distribution
     */
    float getNormalMin()
    {
        return normalDistribution_.min();
    }
    /**
     * Method of getting the biggest number of normal distribution.
     * For testing purposes only.
     * \return maximum possible number that we will get from normal distribution
     */
    float getNormalMax()
    {
        return normalDistribution_.max();
    }
};
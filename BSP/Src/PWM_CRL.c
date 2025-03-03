#include "PWM_CRL.h"

SLAVE_PWM slave_pwm;

/**
 * @brief	控制pwm输出的占空比及频率
 *
 * @remark	Fpwm =Tclk / ((arr+1)*(psc+1)) = 1000000/(arr+1) (单位：Hz)
 *          Tclk = 72M Hz    psc=71    arr = 1000000/frequency - 1
 *
 * @param   pwm1_dutycycle		PWM1占空比
 * @param   pwm2_dutycycle	    PWM2占空比
 * @param   pwm3_dutycycle	    PWM3占空比
 * @param   frequency           频率
 *
 * @return  void
 */
void pwm_crl( uint8_t pwm1_dutycycle, uint8_t pwm2_dutycycle, uint8_t pwm3_dutycycle,uint32_t frequency)
{
    uint32_t Fpwm = 1000000 / frequency - 1;

    /* 1, 设置频率                                            */
    htim4.Instance->ARR  = Fpwm;

    /* 2, 设置PWM1(CCR2)、PWM2(CCR3)、PWM3(CCR4)的占空比      */
    htim4.Instance->CCR2 = ((uint32_t)pwm1_dutycycle * Fpwm) / 100 ;
    htim4.Instance->CCR3 = ((uint32_t)pwm2_dutycycle * Fpwm) / 100 ;
    htim4.Instance->CCR4 = ((uint32_t)pwm3_dutycycle * Fpwm) / 100 ;
}

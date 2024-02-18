#include <iostream>
#include <functional>
#include <vector>

int main()
{
    std::vector<float> ffda_buff(384);
    std::vector<float> rgb_conv(ffda_buff.size() * 2);

    unsigned short m;
    unsigned short m2;
    unsigned int i = 0;
    unsigned int i2 = 0;

    for (unsigned int num_agr = 0; num_agr < ffda_buff.size(); num_agr++)
    {
        ffda_buff[num_agr] = num_agr;
    }

    std::function<void()> i_subsamp = [&i, &m, &m2, &ffda_buff, &rgb_conv]()
    {
        for (unsigned short it = 0; it < 64; it++)
        {
            rgb_conv[(i * 768) + (m * 3) + it] = ffda_buff[(i * 384) + m + it];
        }

        for (unsigned short it = 0; it < 64; it++)
        {
            if (it >= 0 && it < 8 || it >= 16 && it < 24 || it >= 32 && it < 40 || it >= 48 && it < 56)
            {
                rgb_conv[(i * 768) + (m * 3) + 64 + it] = ffda_buff[(i * 384) + m2 + short(it / 2)];
            }

            if (it >= 8 && it < 16 || it >= 24 && it < 32 || it >= 40 && it < 48 || it >= 56)
            {
                rgb_conv[(i * 768) + (m * 3) + 64 + it] = rgb_conv[(i * 768) + (m * 3) + 64 + it - 8];
            }
        }

        for (unsigned short it = 0; it < 64; it++)
        {
            if (it >= 0 && it < 8 || it >= 16 && it < 24 || it >= 32 && it < 40 || it >= 48 && it < 56)
            {
                rgb_conv[(i * 768) + (m * 3) + 128 + it] = ffda_buff[(i * 384) + m2 + 64 + short(it / 2)];
            }

            if (it >= 8 && it < 16 || it >= 24 && it < 32 || it >= 40 && it < 48 || it >= 56)
            {
                rgb_conv[(i * 768) + (m * 3) + 128 + it] = rgb_conv[(i * 768) + (m * 3) + 128 + it - 8];
            }
        }
    };

    for (unsigned int iter_conv = 0; iter_conv < rgb_conv.size() / 768; iter_conv++)
    {
        m = 0;
        m2 = 256;

        i_subsamp();

        m = 64;
        m2 = 260;

        i_subsamp();

        m = 128;
        m2 = 288;

        i_subsamp();

        m = 192;
        m2 = 292;

        i_subsamp();

        i++;
    }
    
    std::cout << "Expanded matrices (1:1:1 ratio)" << std::endl << std::endl;
    
    for (unsigned short cont_64 = 0; cont_64 < rgb_conv.size() / 64; cont_64++)
    {
        for (unsigned short cont_y = 0; cont_y < 8; cont_y++)
        {
            for (unsigned short cont_x = 0; cont_x < 8; cont_x++)
            {
                std::cout << rgb_conv[(cont_64 * 64) + (cont_y * 8) + cont_x] << " ";
            }

            std::cout << "\n";
        }

        std::cout << "\n";
    }

    i = 0;
    i2 = 0;

    if (true)
    {
        std::vector<float> rgb_conv2 = rgb_conv;

        for (unsigned int iter_conv = 0; iter_conv < rgb_conv.size() / 3; iter_conv++)
        {
            rgb_conv[iter_conv * 3] = rgb_conv2[(i2 * 192) + i];
            rgb_conv[(iter_conv * 3) + 1] = rgb_conv2[(i2 * 192) + 64 + i];
            rgb_conv[(iter_conv * 3) + 2] = rgb_conv2[(i2 * 192) + 128 + i];

            i++;

            if (i == 64)
            {
                i = 0;
                i2++;
            }
        }
    }
    
    unsigned short ancho_in = 16;
    unsigned short alto_in = 16;
    
    unsigned short ancho_8;
    unsigned short alto_8;

    if ((ancho_in / 8) - short(ancho_in / 8) > 0)
    {
        ancho_8 = (short(ancho_in / 8) + 1) * 8;
    }
    else
    {
        ancho_8 = ancho_in;
    }

    if ((alto_in / 8) - short(alto_in / 8) > 0)
    {
        alto_8 = (short(alto_in / 8) + 1) * 8;
    }
    else
    {
        alto_8 = alto_in;
    }
    
    if (true)
    {
        std::vector<float> rgb_conv2 = rgb_conv;
        unsigned int iter_lin = 0;
        
        for(unsigned short cuad_y = 0; cuad_y < alto_8 / 16; cuad_y++)
        {
            for(unsigned short cuad_x = 0; cuad_x < ancho_8 / 16; cuad_x++)
            {
                for(unsigned short mcu_y = 0; mcu_y < 2; mcu_y++)
                {
                    for(unsigned short mcu_x = 0; mcu_x < 2; mcu_x++)
                    {
                        for(unsigned short filas_y = 0; filas_y < 8; filas_y++)
                        {
                            for(unsigned short columnas_x = 0; columnas_x < 8; columnas_x++)
                            {
                                for(unsigned short cont_rgb = 0; cont_rgb < 3; cont_rgb++)
                                {
                                    rgb_conv[(cuad_y * (ancho_8 / 16) * 768) + (cuad_x * 48) + (mcu_y * (ancho_8 / 8) * 192) + (mcu_x * 24) + 0 + (filas_y * (ancho_8 * 3)) + (columnas_x * 3) + cont_rgb] = rgb_conv2[iter_lin];
                                    
                                    iter_lin++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    std::cout << "Output pixels" << std::endl << std::endl;
    
    for (unsigned int iter_conv = 0; iter_conv < rgb_conv.size() / 3; iter_conv++)
    {
        std::cout << "[" << rgb_conv[iter_conv * 3] << "," << rgb_conv[(iter_conv * 3) + 1] << "," << rgb_conv[(iter_conv * 3) + 2] << "] ";
    }

    return 0;
}

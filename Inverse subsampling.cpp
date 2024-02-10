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

    for (unsigned int iter_conv = 0; iter_conv < rgb_conv.size() / 3; iter_conv++)
    {
        std::cout << "[" << rgb_conv[iter_conv * 3] << ", " << rgb_conv[(iter_conv * 3) + 1] << ", " << rgb_conv[(iter_conv * 3) + 2] << "] ";
    }

    return 0;
}

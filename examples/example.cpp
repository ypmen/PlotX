/**
 * @author Yunpeng Men
 * @email ypmen@mpifr-bonn.mpg.de
 * @create date 2021-02-26 10:39:44
 * @modify date 2021-02-26 10:39:44
 * @desc [description]
 */

#include <cmath>
#include <ctime>
#include <random>

#include "plotx.h"

int main()
{
    PlotX::Figure fig(8, 0.75);
    fig.set_background_color("black");
    fig.set_defalut_color("white");

    PlotX::Axes ax(0.05, 0.45, 0.55, 0.9);
    ax.label(true, true, true, true);
    ax.frame(true, true, true, true);
    ax.set_title("x = y");
    ax.set_xlabel("x");
    ax.set_ylabel("y");
    
    std::vector<float> x(100), y1(100), y2(100);
    for (long int i=0; i<100; i++)
    {
        x[i] = i+1;
        y1[i] = x[i];
        y2[i] = 100/x[i];
    }
    ax.axvspan(pow(10, 1.5-0.05), pow(10, 1.5+0.05), 0., 1., {{"facecolor", "b"}, {"filled", "none"}});
    ax.axhspan(pow(10, 1.5-0.05), pow(10, 1.5+0.05), 0., 1., {{"facecolor", "y"}});
    ax.plot(x, y1, {{"linestyle", "--"}, {"linewidth", "10"}, {"color", "r"}});
    ax.scatter(x, y2, {{"markersize", "1"}, {"color", "y"}, {"marker", "0"}});
    ax.circle(pow(10, 0.25), pow(10, 1), 5);
    ax.cross(pow(10, 1.75), pow(10, 1), 5);
    ax.point(pow(10, 1.), pow(10, 0.25), 5, "#00ffff", "7");
    ax.annotate("^_^", pow(10, 1.), pow(10, 1.75), {{"xycoords", "data"}, {"rotation", "0"}, {"fontsize", "1"}, {"color", "red"}});
    ax.axvline(pow(10, 0.5), 0., 1., {{"color", "g"}});
    ax.axhline(pow(10, 0.5));
    
    ax.xscale("log");
    ax.yscale("log");

    ax.annotate("PlotX", 0.45, 0.95, {{"xycoords", "figure fraction"}, {"rotation", "0"}, {"fontsize", "2"}, {"color", "red"}});

    ax.autoscale();
    fig.push(ax);

    std::vector<float> xdata(100), ydata(100), zdata(100*100);
    for (long int j=0; j<100; j++)
    {
        ydata[j] = (j-50.)/100.;
    }
    for (long int i=0; i<100; i++)
    {
        xdata[i] = (i-50.)/100.;
    }
    
    for (long int j=0; j<100; j++)
    {
        for (long int i=0; i<100; i++)
        {
            zdata[j*100+i] = exp(-xdata[i]*xdata[i]-ydata[j]*ydata[j]);
        }
    }

    std::vector<std::vector<float>> cm_data = {
        {0.267004, 0.283072, 0.262138, 0.220057, 0.177423, 0.143343, 0.119512, 0.166383, 0.319809, 0.525776, 0.762373, 0.993248},
        {0.004874, 0.130895, 0.242286, 0.343307, 0.437527, 0.522773, 0.607464, 0.690856, 0.770914, 0.833491, 0.876424, 0.906157},
        {0.329415, 0.449241, 0.520837, 0.549413, 0.557565, 0.556295, 0.540218, 0.496502, 0.411152, 0.288127, 0.137064, 0.143936}
    };

    PlotX::Axes ax2(0.55, 0.95, 0.55, 0.9);
    ax2.pcolor(xdata, ydata, zdata, "viridis", {});
    ax2.minorticks_off();
    ax2.majorticks_off();
    ax2.set_xlabel("\\gd");
    ax2.set_ylabel("\\gh");
    ax2.set_title("\\gy = e\\u-\\gd\\u2\\d-\\gh\\u2");
    fig.push(ax2);

    std::vector<float> xdata1={1, 2, 3, 4};
    std::vector<float> ydata1={1, 1, 1, 1};
    std::vector<float> xerr1={0.1, 0.1, 0.1, 0.1};

    std::vector<float> ydata2={2, 2, 2, 2};
    std::vector<float> yerr2={0.1, 0.1, 0.1, 0.1};

    std::vector<float> ydata3={3, 3, 3, 3};
    std::vector<float> xerr3={0.4, 0.4, 0.4, 0.4}, yerr3={0.1, 0.1, 0.1, 0.1};
    
    std::vector<float> ydata4={4, 4, 4, 4};
    std::vector<std::vector<float>> xerr4={{0.1, 0.1, 0.1, 0.1}, {0.2, 0.2, 0.2, 0.2}}, yerr4={{0.1, 0.1, 0.1, 0.1}, {0.2, 0.2, 0.2, 0.2}};

    std::vector<float> ydata5={5, 5, 5, 5};
    std::vector<float> ydata6={6, 6, 6, 6};

    PlotX::Axes ax3(0.05, 0.45, 0.05, 0.45);
    ax3.errorbar(xdata1, ydata1, {}, xerr1, {{"color", "red"}, {"linewidth", "5"}, {"capsize", "2"}});
    ax3.errorbar(xdata1, ydata2, yerr2, {}, {{"color", "y"}, {"linewidth", "5"}, {"capsize", "3"}});
    ax3.errorbar(xdata1, ydata3, yerr3, xerr3);
    ax3.errorbar(xdata1, ydata4, yerr4, xerr4);
    ax3.errorbar(xdata1, ydata5, {}, xerr1, {{"xlolims", ""}});
    ax3.errorbar(xdata1, ydata6, yerr2, {}, {{"uplims", ""}});

    ax3.xscale("log");

    fig.push(ax3);

    std::vector<float> histdata(1000);
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(0., 1.0);
    for (auto d=histdata.begin(); d!=histdata.end(); ++d)
    {
        *d = distribution(generator);
    }

    std::vector<float> stepx(20);
    std::vector<float> stepy(20);
    for (long int i=0; i<20; i++)
    {
        stepx[i] = 6.*(i-10.)/20.;
        stepy[i] = 300+100*std::exp(-0.5*stepx[i]*stepx[i]);
    }

    PlotX::Axes ax4(0.55, 0.95, 0.05, 0.45);
    ax4.hist(histdata, 10, {0., 0.}, {{"histtype", "barfilled"}, {"linestyle", "-"}, {"linewidth", "1"}, {"color", "b"}});
    ax4.hist(histdata, 10, {0., 0.}, {{"histtype", "bar"}, {"linestyle", "-"}, {"linewidth", "1"}, {"color", "red"}});
    ax4.step(stepx, stepy);

    fig.push(ax4);

    fig.show();

    fig.save("example.png/PNG");

    return 0;
}
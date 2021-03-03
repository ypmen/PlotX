/**
 * @author Yunpeng Men
 * @email ypmen@mpifr-bonn.mpg.de
 * @create date 2021-02-27 12:31:54
 * @modify date 2021-02-27 12:31:54
 * @desc [This is a c++ wrapper for pgplot]
 */

#ifndef PLOTX_H
#define PLOTX_H

#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <sstream>

#include "cpgplot.h"

namespace PlotX
{
    int get_linestyle(const std::string &linestyle);
    int get_linewidth(const std::string &linewidth);
    int get_symbol(const std::string &symbol);
    float get_height(const std::string &height);
    int get_color(const std::string &color);
    int get_filled(const std::string &filled);
    void get_rgb(float &r, float &g, float &b, const std::string &color);
    void set_cmap(const std::string &cmap, const std::vector<std::vector<float>> &cm_data=std::vector<std::vector<float>>());
    class PPlot
    {
    public:
        PPlot(){_autoscalex = false; _autoscaley = false;}
        virtual ~PPlot(){}
        virtual PPlot & draw(bool logx, bool logy){return *this;};
        virtual PPlot * clone()
        {
            PPlot *plot = new PPlot;
            *plot = *this;
            return plot;
        }
        virtual void get_win(float &xmin, float &xmax, float &ymin, float &ymax){}
        virtual PPlot & autoscale(bool autoscalex, bool autoscaley)
        {
            _autoscalex = autoscalex;
            _autoscaley = autoscaley;

            return *this;
        }
    public:
        bool _autoscalex;
        bool _autoscaley;
    };

    class PLine : public PPlot
    {
    public:
        PLine(){}
        ~PLine(){}
    public:
        PLine & draw(bool logx=false, bool logy=false);
        PLine * clone();
        void get_win(float &xmin, float &xmax, float &ymin, float &ymax);
    public:
        std::vector<float> xdata;
        std::vector<float> ydata;
        std::string linestyle;
        std::string linewidth;
        std::string color;
    };

    class PAxvline : public PPlot
    {
    public:
        PAxvline(){x = 0.; _ymin = 0.; _ymax = 0.;}
        ~PAxvline(){}
    public:
        PAxvline & draw(bool logx=false, bool logy=false);
        PAxvline * clone();
        void get_win(float &xmin, float &xmax, float &ymin, float &ymax);
    public:
        float x;
        float _ymin;
        float _ymax;
        std::string linestyle;
        std::string linewidth;
        std::string color;
    };

    class PAxhline : public PPlot
    {
    public:
        PAxhline(){y = 0.; _xmin = 0.; _xmax = 0.;}
        ~PAxhline(){}
    public:
        PAxhline & draw(bool logx=false, bool logy=false);
        PAxhline * clone();
        void get_win(float &xmin, float &xmax, float &ymin, float &ymax);
    public:
        float y;
        float _xmin;
        float _xmax;
        std::string linestyle;
        std::string linewidth;
        std::string color;
    };

    class PAxvspan : public PPlot
    {
    public:
        PAxvspan(){_xmin = 0.; _xmax = 0.; _ymin = 0.; _ymax = 0.;}
        ~PAxvspan(){}
    public:
        PAxvspan & draw(bool logx=false, bool logy=false);
        PAxvspan * clone();
        void get_win(float &xmin, float &xmax, float &ymin, float &ymax);
    public:
        float _xmin;
        float _xmax;
        float _ymin;
        float _ymax;
        std::string linestyle;
        std::string linewidth;
        std::string color;
        std::string filled;
    };

    class PAxhspan : public PPlot
    {
    public:
        PAxhspan(){_ymin = 0.; _ymax = 0.; _xmin = 0.; _xmax = 0.;}
        ~PAxhspan(){}
    public:
        PAxhspan & draw(bool logx=false, bool logy=false);
        PAxhspan * clone();
        void get_win(float &xmin, float &xmax, float &ymin, float &ymax);
    public:
        float _ymin;
        float _ymax;
        float _xmin;
        float _xmax;
        std::string linestyle;
        std::string linewidth;
        std::string color;
        std::string filled;
    };

    class PScatter : public PPlot
    {
    public:
        PScatter(){}
        ~PScatter(){}
    public:
        PScatter & draw(bool logx=false, bool logy=false);
        PScatter * clone();
        void get_win(float &xmin, float &xmax, float &ymin, float &ymax);
    public:
        std::vector<float> xdata;
        std::vector<float> ydata;
        std::string marker;
        std::string markersize;
        std::string color;
    };

    class PPoint : public PPlot
    {
    public:
        PPoint(){x = 0.; y = 0.;}
        ~PPoint(){}
    public:
        PPoint & draw(bool logx=false, bool logy=false);
        PPoint * clone();
        void get_win(float &xmin, float &xmax, float &ymin, float &ymax);
    public:
        float x;
        float y;
        std::string marker;
        std::string markersize;
        std::string color;
    };

    class PPcolor : public PPlot
    {
    public:
        PPcolor(){_autoscalex = true; _autoscaley=true;};
        ~PPcolor(){};
    public:
        PPcolor & draw(bool logx=false, bool logy=false);
        PPcolor * clone();
        void get_win(float &xmin, float &xmax, float &ymin, float &ymax);
        PPcolor & autoscale(bool autoscalex, bool autoscaley)
        {
            _autoscalex = true;
            _autoscaley = true;

            return *this;
        }
    public:
        std::vector<float> xdata;
        std::vector<float> ydata;
        std::vector<float> zdata;
        std::string cmap;
        std::vector<std::vector<float>> cm_data;
    };

    class PErrorbar : public PPlot
    {
    public:
        PErrorbar(){capsize = "1"; uplims = "True"; lolims = "True"; xuplims = "True"; xlolims = "True";}
        ~PErrorbar(){}
    public:
        PErrorbar & draw(bool logx=false, bool logy=false);
        PErrorbar * clone();
        void get_win(float &xmin, float &xmax, float &ymin, float &ymax);
    public:
        std::vector<float> xdata;
        std::vector<float> ydata;
        std::vector<std::vector<float>> xerr;
        std::vector<std::vector<float>> yerr;
        std::string capsize;
        std::string linewidth;
        std::string color;
        std::string uplims;
        std::string lolims;
        std::string xuplims;
        std::string xlolims;
    };
    class PText : public PPlot
    {
    public:
        PText(){x = 0.; y = 0.; xycoords = "axes fraction"; rotation="0"; refpos="left";}
        ~PText(){}
    public:
        PText & draw(bool logx=false, bool logy=false);
        PText * clone();
        void get_win(float &xmin, float &xmax, float &ymin, float &ymax){}
    public:
        float x;
        float y;
        std::string text;
        std::string xycoords;
        std::string fontsize;
        std::string color;
        std::string rotation;
        std::string refpos;
    };

    class PHistogram : public PPlot
    {
    public:
        PHistogram(){_autoscaley=true; histtype="bar";}
        ~PHistogram(){}
    public:
        PHistogram & draw(bool logx=false, bool logy=false);
        PHistogram * clone();
        void get_win(float &xmin, float &xmax, float &ymin, float &ymax);
        PHistogram & autoscale(bool autoscalex, bool autoscaley)
        {
            _autoscalex = autoscalex;
            _autoscaley = true;

            return *this;
        }
    public:
        std::vector<float> data;
        float datmin;
        float datmax;
        int nbin;
        std::string histtype; /* bar,barfilled,step */
        std::string linestyle;
        std::string linewidth;
        std::string color;
    };

    class PStep : public PPlot
    {
    public:
        PStep(){where = "mid";}
        ~PStep(){}
    public:
        PStep & draw(bool logx=false, bool logy=false);
        PStep * clone();
        void get_win(float &xmin, float &xmax, float &ymin, float &ymax);
    public:
        std::vector<float> xdata;
        std::vector<float> ydata;
        std::string where;
        std::string linestyle;
        std::string linewidth;
        std::string color;
    };

    class Axes
    {
    public:
        Axes();
        Axes(const Axes &ax);
        Axes & operator=(const Axes &ax);
        Axes(float left, float right, float bottom, float top);
        ~Axes();
        Axes & frame(bool frameleft, bool frameright, bool framebottom, bool frametop)
        {
            _frameleft = frameleft;
            _frameright = frameright;
            _framebottom = framebottom;
            _frametop = frametop;

            return *this;
        }
        Axes & grid(bool gridhorizontal, bool gridvertical)
        {
            _gridvertical = gridvertical;
            _gridhorizontal = gridhorizontal;

            return *this;
        }
        Axes & label(bool labelleft, bool labelright, bool labelbottom, bool labeltop, bool labelrotation=false)
        {
            _labelleft = labelleft;
            _labelright = labelright;
            _labelbottom = labelbottom;
            _labeltop = labeltop;

            _labelrotation = labelrotation;

            return *this;
        }
        Axes & xscale(const std::string &val)
        {
            if (val == "log")
                _logx = true;
            else
                _logx = false;

            return *this;
        }
        Axes & yscale(const std::string &val)
        {
            if (val == "log")
                _logy = true;
            else
                _logy = false;

            return *this;
        }
        Axes & minorticks_on(){_minorticks = true; return *this;}
        Axes & minorticks_off(){_minorticks = false; return *this;}
        Axes & majorticks_on(){_majorticks = true; return *this;}
        Axes & majorticks_off(){_majorticks = false; return *this;}
        Axes & axis_on(){_axis = true; return *this;}
        Axes & axis_off(){_axis = false; return *this;}        
    public:
        Axes & set_xlabel(const std::string &xlabel){_xlabel = xlabel; return *this;}
        Axes & set_ylabel(const std::string &ylabel){_ylabel = ylabel; return *this;}
        Axes & set_title(const std::string &title){_title = title; return *this;}
    public:
        Axes & plot(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::map<std::string, std::string> &options=std::map<std::string, std::string>());
        Axes & axvline(float x, float ymin=0., float ymax=1., const std::map<std::string, std::string> &options=std::map<std::string, std::string>());
        Axes & axhline(float y, float xmin=0., float xmax=1., const std::map<std::string, std::string> &options=std::map<std::string, std::string>());
        Axes & axvspan(float xmin, float xmax, float ymin=0., float ymax=1., const std::map<std::string, std::string> &options=std::map<std::string, std::string>());
        Axes & axhspan(float ymin, float ymax, float xmin=0., float xmax=1., const std::map<std::string, std::string> &options=std::map<std::string, std::string>());
        Axes & scatter(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::map<std::string, std::string> &options=std::map<std::string, std::string>());
        Axes & circle(float x, float y, float size=1., const std::string &color="red");
        Axes & cross(float x, float y, float size=1., const std::string &color="red");
        Axes & point(float x, float y, float size=1., const std::string &color="red", const std::string &marker="1");
        Axes & pcolor(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::vector<float> &zdata, const std::string &cmap="", const std::vector<std::vector<float>> &cm_data=std::vector<std::vector<float>>(), const std::map<std::string, std::string> &options=std::map<std::string, std::string>());
        Axes & annotate(const std::string &text, float x, float y, const std::map<std::string, std::string> &options=std::map<std::string, std::string>());
        Axes & errorbar(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::vector<std::vector<float>> &yerr, const std::vector<std::vector<float>> &xerr=std::vector<std::vector<float>>(), const std::map<std::string, std::string> &options=std::map<std::string, std::string>());
        Axes & errorbar(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::vector<float> &yerr, const std::vector<float> &xerr=std::vector<float>(), const std::map<std::string, std::string> &options=std::map<std::string, std::string>());
        Axes & hist(const std::vector<float> &data, int bins=10, const std::pair<float, float> &range={0., 0.}, const std::map<std::string, std::string> &options=std::map<std::string, std::string>());
        Axes & step(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::map<std::string, std::string> &options=std::map<std::string, std::string>());

        Axes & autoscale(bool enable = true, std::string axis="both", bool tight=true);
        Axes & draw();
    private:
        void get_opt(std::string &xopt, std::string &yopt);
        void get_win();
    private:
        std::vector<std::unique_ptr<PPlot>> plots;
    private:
        /* viewport */
        float _left;
        float _right;
        float _bottom;
        float _top;

        /* world coordinate*/
        float _xmin;
        float _xmax;
        float _ymin;
        float _ymax;

        /* tick params */
        bool _directionout;

        bool _labeltop;
        bool _labelbottom;
        bool _labelleft;
        bool _labelright;

        bool _labelrotation;
        
        bool _minorticks;
        bool _majorticks;

        bool _frametop;
        bool _framebottom;
        bool _frameleft;
        bool _frameright;

        bool _gridvertical;
        bool _gridhorizontal;

        bool _logx;
        bool _logy;

        bool _axis;

        bool _autoscalex;
        bool _autoscaley;

        std::string _xlabel;
        std::string _ylabel;
        std::string _title;
    };

    class Figure
    {
    public:
        Figure();
        Figure(float width, float aspect);
        ~Figure();
        Figure & push(const Axes &ax);
        Figure & set_background_color(const std::string &color){ _background_color = color; return *this;}
        Figure & set_defalut_color(const std::string &color){_defalut_color = color; return *this;}
        Figure & show();
        Figure & save(const std::string &fname);
    private:
        float _width;
        float _aspect;
        std::string _background_color;
        std::string _defalut_color;
        std::vector<Axes> axes;
    };
}

#endif /* PLOTX_H */

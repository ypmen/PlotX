/**
 * @author Yunpeng Men
 * @email ypmen@mpifr-bonn.mpg.de
 * @create date 2021-02-27 13:16:32
 * @modify date 2021-02-27 13:16:32
 * @desc [description]
 */

#include "config.h"

#include <iostream>
#include <algorithm>
#include <limits>
#include <cmath>
#include <stdio.h>
#include <string.h>

#ifdef HAVE_LIBCFITSIO
    #include <fitsio.h>
#endif

#include "plotx.h"

using namespace PlotX;

/*================== Axes =================*/
Axes::Axes()
{
    cnt = 0;
    id = 0;

    _left = 0.;
    _right = 1.;
    _bottom = 0.;
    _top = 1;

    _xmin = std::numeric_limits<float>::max();
    _xmax = std::numeric_limits<float>::min();
    _ymin = std::numeric_limits<float>::max();
    _ymax = std::numeric_limits<float>::min();

    _directionout = false;

    _labeltop = false;
    _labelbottom = true;
    _labelleft = true;
    _labelright = false;

    _labelrotation = false;
        
    _minorticks = true;
    _majorticks = true;

    _frametop = true;
    _framebottom = true;
    _frameleft = true;
    _frameright = true;

    _gridvertical = false;
    _gridhorizontal = false;

    _logx = false;
    _logy = false;

    _axis = false;

    _autoscalex = false;
    _autoscaley = false;

    _xlimits = false;
    _ylimits = false;

    _fontsize_label = "1";
    _fontsize_ticklabel = "1";
}

Axes::Axes(float left, float right, float bottom, float top)
{
    cnt = 0;
    id = 0;

    _left = left;
    _right = right;
    _bottom = bottom;
    _top = top;

    _xmin = std::numeric_limits<float>::max();
    _xmax = std::numeric_limits<float>::min();
    _ymin = std::numeric_limits<float>::max();
    _ymax = std::numeric_limits<float>::min();

    _directionout = false;

    _labeltop = false;
    _labelbottom = true;
    _labelleft = true;
    _labelright = false;

    _labelrotation = false;
        
    _minorticks = true;
    _majorticks = true;

    _frametop = true;
    _framebottom = true;
    _frameleft = true;
    _frameright = true;

    _gridvertical = false;
    _gridhorizontal = false;

    _logx = false;
    _logy = false;

    _autoscalex = false;
    _autoscaley = false;

    _axis = false;

    _xlimits = false;
    _ylimits = false;

    _fontsize_label = "1";
    _fontsize_ticklabel = "1";
}

Axes::Axes(const Axes &ax)
{
    cnt = ax.cnt;
    id = ax.id;

    for (auto p=ax.plots.begin(); p!=ax.plots.end(); ++p)
    {
        plots.push_back(std::unique_ptr<PPlot>(p->get()->clone()));
    }

    _left = ax._left;
    _right = ax._right;
    _bottom = ax._bottom;
    _top = ax._top;

    _xmin = ax._xmin;
    _xmax = ax._xmax;
    _ymin = ax._ymin;
    _ymax = ax._ymax;

    _directionout = ax._directionout;

    _labeltop = ax._labeltop;
    _labelbottom = ax._labelbottom;
    _labelleft = ax._labelleft;
    _labelright = ax._labelright;

    _labelrotation = ax._labelrotation;
        
    _minorticks = ax._minorticks;
    _majorticks = ax._majorticks;

    _frametop = ax._frametop;
    _framebottom = ax._framebottom;
    _frameleft = ax._frameleft;
    _frameright = ax._frameright;

    _gridvertical = ax._gridvertical;
    _gridhorizontal = ax._gridhorizontal;

    _logx = ax._logx;
    _logy = ax._logy;

    _axis = ax._axis;

    _autoscalex = ax._autoscalex;
    _autoscaley = ax._autoscaley;

    _xlimits = ax._xlimits;
    _ylimits = ax._ylimits;

    _xlabel = ax._xlabel;
    _ylabel = ax._ylabel;
    _title = ax._title;

    _fontsize_label = ax._fontsize_label;
    _fontsize_ticklabel = ax._fontsize_ticklabel;
}

Axes & Axes::operator=(const Axes &ax)
{
    cnt = ax.cnt;
    id = ax.id;

    for (auto p=ax.plots.begin(); p!=ax.plots.end(); ++p)
    {
        plots.push_back(std::unique_ptr<PPlot>(p->get()->clone()));
    }

    _left = ax._left;
    _right = ax._right;
    _bottom = ax._bottom;
    _top = ax._top;

    _xmin = ax._xmin;
    _xmax = ax._xmax;
    _ymin = ax._ymin;
    _ymax = ax._ymax;

    _directionout = ax._directionout;

    _labeltop = ax._labeltop;
    _labelbottom = ax._labelbottom;
    _labelleft = ax._labelleft;
    _labelright = ax._labelright;

    _labelrotation = ax._labelrotation;
        
    _minorticks = ax._minorticks;
    _majorticks = ax._majorticks;

    _frametop = ax._frametop;
    _framebottom = ax._framebottom;
    _frameleft = ax._frameleft;
    _frameright = ax._frameright;

    _gridvertical = ax._gridvertical;
    _gridhorizontal = ax._gridhorizontal;

    _logx = ax._logx;
    _logy = ax._logy;

    _axis = ax._axis;

    _autoscalex = ax._autoscalex;
    _autoscaley = ax._autoscaley;

    _xlimits = ax._xlimits;
    _ylimits = ax._ylimits;

    _xlabel = ax._xlabel;
    _ylabel = ax._ylabel;
    _title = ax._title;

    _fontsize_label = ax._fontsize_label;
    _fontsize_ticklabel = ax._fontsize_ticklabel;

    return *this;   
}

Axes::~Axes(){}

Axes & Axes::plot(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::map<std::string, std::string> &options)
{
    if (xdata.size() != ydata.size())
    {
        std::cerr<<"Error: xdata and ydata size not equal"<<std::endl;
        return *this;
    }

    std::unique_ptr<PLine> line(new PLine);
    line->axesid = id;
    line->id = ++cnt;
    line->xdata = xdata;
    line->ydata = ydata;

    for (auto item=options.begin(); item!=options.end(); ++item)
    {
        if (item->first == "linestyle")
        {
            line->linestyle = item->second;
        }
        else if (item->first == "linewidth")
        {
            line->linewidth = item->second;
        }
        else if (item->first == "color")
        {
            line->color = item->second;
        }
        else
        {
            std::cerr<<"Warning: attribute not supported"<<std::endl;
        }
    }

    plots.push_back(std::move(line));

    return *this;
}

Axes & Axes::plot(const std::vector<float> &ydata, const std::map<std::string, std::string> &options)
{
    std::unique_ptr<PLine> line(new PLine);
    line->axesid = id;
    line->id = ++cnt;
    line->xdata.resize(ydata.size());
    std::iota(line->xdata.begin(), line->xdata.end(), 0);
    line->ydata = ydata;

    for (auto item=options.begin(); item!=options.end(); ++item)
    {
        if (item->first == "linestyle")
        {
            line->linestyle = item->second;
        }
        else if (item->first == "linewidth")
        {
            line->linewidth = item->second;
        }
        else if (item->first == "color")
        {
            line->color = item->second;
        }
        else
        {
            std::cerr<<"Warning: attribute not supported"<<std::endl;
        }
    }

    plots.push_back(std::move(line));

    return *this;
}

Axes & Axes::axvline(float x, float ymin, float ymax, const std::map<std::string, std::string> &options)
{
    if (ymin < 0.)
    {
        std::cerr<<"Warning: ymin range is 0-1"<<std::endl;
        ymin = 0.;
    }
    if (ymax > 1.)
    {
        std::cerr<<"Warning: ymax range is 0-1"<<std::endl;
        ymax = 1.;
    }
    if (ymin > 1.)
    {
        std::cerr<<"Warning: ymin range is 0-1"<<std::endl;
        ymin = 1.;
    }
    if (ymax < 0.)
    {
        std::cerr<<"Warning: ymax range is 0-1"<<std::endl;
        ymax = 0.;
    }

    std::unique_ptr<PAxvline> line(new PAxvline);
    line->axesid = id;
    line->id = ++cnt;
    line->x = x;
    line->_ymin = ymin;
    line->_ymax = ymax;

    for (auto item=options.begin(); item!=options.end(); ++item)
    {
        if (item->first == "linestyle")
        {
            line->linestyle = item->second;
        }
        else if (item->first == "linewidth")
        {
            line->linewidth = item->second;
        }
        else if (item->first == "color")
        {
            line->color = item->second;
        }
        else
        {
            std::cerr<<"Warning: attribute not supported"<<std::endl;
        }
    }

    plots.push_back(std::move(line));

    return *this;
}

Axes & Axes::axhline(float y, float xmin, float xmax, const std::map<std::string, std::string> &options)
{
    if (xmin < 0.)
    {
        std::cerr<<"Warning: xmin range is 0-1"<<std::endl;
        xmin = 0.;
    }
    if (xmax > 1.)
    {
        std::cerr<<"Warning: xmax range is 0-1"<<std::endl;
        xmax = 1.;
    }
    if (xmin > 1.)
    {
        std::cerr<<"Warning: xmin range is 0-1"<<std::endl;
        xmin = 1.;
    }
    if (xmax < 0.)
    {
        std::cerr<<"Warning: xmax range is 0-1"<<std::endl;
        xmax = 0.;
    }

    std::unique_ptr<PAxhline> line(new PAxhline);
    line->axesid = id;
    line->id = ++cnt;
    line->y = y;
    line->_xmin = xmin;
    line->_xmax = xmax;

    for (auto item=options.begin(); item!=options.end(); ++item)
    {
        if (item->first == "linestyle")
        {
            line->linestyle = item->second;
        }
        else if (item->first == "linewidth")
        {
            line->linewidth = item->second;
        }
        else if (item->first == "color")
        {
            line->color = item->second;
        }
        else
        {
            std::cerr<<"Warning: attribute not supported"<<std::endl;
        }
    }

    plots.push_back(std::move(line));

    return *this;
}

Axes & Axes::axvspan(float xmin, float xmax, float ymin, float ymax, const std::map<std::string, std::string> &options)
{
    if (ymin < 0.)
    {
        std::cerr<<"Warning: ymin range is 0-1"<<std::endl;
        ymin = 0.;
    }
    if (ymax > 1.)
    {
        std::cerr<<"Warning: ymax range is 0-1"<<std::endl;
        ymax = 1.;
    }
    if (ymin > 1.)
    {
        std::cerr<<"Warning: ymin range is 0-1"<<std::endl;
        ymin = 1.;
    }
    if (ymax < 0.)
    {
        std::cerr<<"Warning: ymax range is 0-1"<<std::endl;
        ymax = 0.;
    }

    std::unique_ptr<PAxvspan> span(new PAxvspan);
    span->axesid = id;
    span->id = ++cnt;
    span->_xmin = xmin;
    span->_xmax = xmax;
    span->_ymin = ymin;
    span->_ymax = ymax;

    for (auto item=options.begin(); item!=options.end(); ++item)
    {
        if (item->first == "linestyle")
        {
            span->linestyle = item->second;
        }
        else if (item->first == "linewidth")
        {
            span->linewidth = item->second;
        }
        else if (item->first == "color" or item->first == "facecolor")
        {
            span->color = item->second;
        }
        else if (item->first == "filled")
        {
            span->filled = item->second;
        }
        else
        {
            std::cerr<<"Warning: attribute not supported"<<std::endl;
        }
    }

    plots.push_back(std::move(span));

    return *this;    
}

Axes & Axes::axhspan(float ymin, float ymax, float xmin, float xmax, const std::map<std::string, std::string> &options)
{
    if (xmin < 0.)
    {
        std::cerr<<"Warning: xmin range is 0-1"<<std::endl;
        xmin = 0.;
    }
    if (xmax > 1.)
    {
        std::cerr<<"Warning: xmax range is 0-1"<<std::endl;
        xmax = 1.;
    }
    if (xmin > 1.)
    {
        std::cerr<<"Warning: xmin range is 0-1"<<std::endl;
        xmin = 1.;
    }
    if (xmax < 0.)
    {
        std::cerr<<"Warning: xmax range is 0-1"<<std::endl;
        xmax = 0.;
    }

    std::unique_ptr<PAxhspan> span(new PAxhspan);
    span->axesid = id;
    span->id = ++cnt;
    span->_ymin = ymin;
    span->_ymax = ymax;
    span->_xmin = xmin;
    span->_xmax = xmax;

    span->filled = "true";

    for (auto item=options.begin(); item!=options.end(); ++item)
    {
        if (item->first == "linestyle")
        {
            span->linestyle = item->second;
        }
        else if (item->first == "linewidth")
        {
            span->linewidth = item->second;
        }
        else if (item->first == "color" or item->first == "facecolor")
        {
            span->color = item->second;
        }
        else if (item->first == "filled")
        {
            span->filled = item->second;
        }
        else
        {
            std::cerr<<"Warning: attribute not supported"<<std::endl;
        }
    }

    plots.push_back(std::move(span));

    return *this;    
}

Axes & Axes::scatter(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::map<std::string, std::string> &options)
{
    if (xdata.size() != ydata.size())
    {
        std::cerr<<"Error: xdata and ydata size not equal"<<std::endl;
        return *this;
    }

    std::unique_ptr<PScatter> scatter(new PScatter);
    scatter->axesid = id;
    scatter->id = ++cnt;
    scatter->xdata = xdata;
    scatter->ydata = ydata;

    for (auto item=options.begin(); item!=options.end(); ++item)
    {
        if (item->first == "marker")
        {
            scatter->marker = item->second;
        }
        else if (item->first == "markersize")
        {
            scatter->markersize = item->second;
        }
        else if (item->first == "color")
        {
            scatter->color = item->second;
        }
        else
        {
            std::cerr<<"Warning: attribute not supported"<<std::endl;
        }
    }

    plots.push_back(std::move(scatter));

    return *this;
}

Axes & Axes::circle(float x, float y, float size, const std::string &color)
{
    std::unique_ptr<PPoint> circle(new PPoint);
    circle->axesid = id;
    circle->id = ++cnt;
    circle->x = x;
    circle->y = y;
    circle->marker = "4";
    circle->markersize = std::to_string(size);
    circle->color = color;

    plots.push_back(std::move(circle));

    return *this;
}

Axes & Axes::cross(float x, float y, float size, const std::string &color)
{
    std::unique_ptr<PPoint> cross(new PPoint);
    cross->axesid = id;
    cross->id = ++cnt;
    cross->x = x;
    cross->y = y;
    cross->marker = "2";
    cross->markersize = std::to_string(size);
    cross->color = color;

    plots.push_back(std::move(cross));

    return *this;
}

Axes & Axes::point(float x, float y, float size, const std::string &color, const std::string &marker)
{
    std::unique_ptr<PPoint> point(new PPoint);
    point->axesid = id;
    point->id = ++cnt;
    point->x = x;
    point->y = y;
    point->marker = marker;
    point->markersize = std::to_string(size);
    point->color = color;

    plots.push_back(std::move(point));

    return *this;
}

Axes & Axes::pcolor(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::vector<float> &zdata, const std::string &cmap, const std::vector<std::vector<float>> &cm_data, const std::map<std::string, std::string> &options)
{
    std::unique_ptr<PPcolor> image(new PPcolor);
    image->axesid = id;
    image->id = ++cnt;
    image->xdata = xdata;
    image->ydata = ydata;
    image->zdata = zdata;
    image->cmap = cmap;
    image->cm_data = cm_data;

    plots.push_back(std::move(image));

    return *this;
}

Axes & Axes::pcolor(size_t xlen, size_t ylen, const std::vector<float> &zdata, const std::string &cmap, const std::vector<std::vector<float>> &cm_data, const std::map<std::string, std::string> &options)
{
    std::unique_ptr<PPcolor> image(new PPcolor);
    image->axesid = id;
    image->id = ++cnt;
    image->xdata.resize(xlen);
    std::iota(image->xdata.begin(), image->xdata.end(), 0);
    image->ydata.resize(ylen);
    std::iota(image->ydata.begin(), image->ydata.end(), 0);
    image->zdata = zdata;
    image->cmap = cmap;
    image->cm_data = cm_data;

    plots.push_back(std::move(image));

    return *this;
}

Axes & Axes::annotate(const std::string &text, float x, float y, const std::map<std::string, std::string> &options)
{
    std::unique_ptr<PText> txt(new PText);
    txt->axesid = id;
    txt->id = ++cnt;
    txt->text = text;
    txt->x = x;
    txt->y = y;

    for (auto item=options.begin(); item!=options.end(); ++item)
    {
        if (item->first == "xycoords")
        {
            txt->xycoords = item->second;
        }
        else if (item->first == "fontsize")
        {
            txt->fontsize = item->second;
        }
        else if (item->first == "rotation")
        {
            txt->rotation = item->second;
        }
        else if (item->first == "color" or item->first == "fontcolor")
        {
            txt->color = item->second;
        }
        else if (item->first == "refpos")
        {
            txt->refpos = item->second;
        }
        else if (item->first == "linewidth")
        {
            txt->linewidth = item->second;
        }
        else
        {
            std::cerr<<"Warning: attribute not supported"<<std::endl;
        }
    }

    plots.push_back(std::move(txt));

    return *this;
}

Axes & Axes::errorbar(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::vector<std::vector<float>> &yerr, const std::vector<std::vector<float>> &xerr, const std::map<std::string, std::string> &options)
{
    if (xdata.size() != ydata.size())
    {
        std::cerr<<"Error: xdata and ydata size not equal"<<std::endl;
        return *this;
    }

    std::unique_ptr<PErrorbar> errbar(new PErrorbar);
    errbar->axesid = id;
    errbar->id = ++cnt;
    errbar->xdata = xdata;
    errbar->ydata = ydata;
    errbar->xerr = xerr;
    errbar->yerr = yerr;

    for (auto item=options.begin(); item!=options.end(); ++item)
    {
        if (item->first == "capsize")
        {
            errbar->capsize = item->second;
        }
        else if (item->first == "linewidth" or item->first == "elinewidth")
        {
            errbar->linewidth = item->second;
        }
        else if (item->first == "color" or item->first == "ecolor")
        {
            errbar->color = item->second;
        }
        else if (item->first == "uplims" or item->first == "yuplims")
        {
            errbar->uplims = item->second;
        }
        else if (item->first == "lolims" or item->first == "ylolims")
        {
            errbar->lolims = item->second;
        }
        else if (item->first == "xuplims")
        {
            errbar->xuplims = item->second;
        }
        else if (item->first == "xlolims")
        {
            errbar->xlolims = item->second;
        }
        else
        {
            std::cerr<<"Warning: attribute not supported"<<std::endl;
        }
    }

    plots.push_back(std::move(errbar));

    return *this;
}

Axes & Axes::errorbar(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::vector<float> &yerr, const std::vector<float> &xerr, const std::map<std::string, std::string> &options)
{
    if (xdata.size() != ydata.size())
    {
        std::cerr<<"Error: xdata and ydata size not equal"<<std::endl;
        return *this;
    }

    std::unique_ptr<PErrorbar> errbar(new PErrorbar);
    errbar->axesid = id;
    errbar->id = ++cnt;
    errbar->xdata = xdata;
    errbar->ydata = ydata;
    if (!xerr.empty())
        errbar->xerr.push_back(xerr);
    if (!yerr.empty())
        errbar->yerr.push_back(yerr);

    for (auto item=options.begin(); item!=options.end(); ++item)
    {
        if (item->first == "capsize")
        {
            errbar->capsize = item->second;
        }
        else if (item->first == "linewidth" or item->first == "elinewidth")
        {
            errbar->linewidth = item->second;
        }
        else if (item->first == "color" or item->first == "ecolor")
        {
            errbar->color = item->second;
        }
        else if (item->first == "uplims" or item->first == "yuplims")
        {
            errbar->uplims = item->second;
        }
        else if (item->first == "lolims" or item->first == "ylolims")
        {
            errbar->lolims = item->second;
        }
        else if (item->first == "xuplims")
        {
            errbar->xuplims = item->second;
        }
        else if (item->first == "xlolims")
        {
            errbar->xlolims = item->second;
        }
        else
        {
            std::cerr<<"Warning: attribute not supported"<<std::endl;
        }
    }

    plots.push_back(std::move(errbar));

    return *this;
}

Axes & Axes::hist(const std::vector<float> &data, int bins, const std::pair<float, float> &range, const std::map<std::string, std::string> &options)
{
    std::unique_ptr<PHistogram> histo(new PHistogram);
    histo->axesid = id;
    histo->id = ++cnt;
    histo->data = data;
    histo->nbin = bins;
    histo->datmin = range.first;
    histo->datmax = range.second;

    for (auto item=options.begin(); item!=options.end(); ++item)
    {
        if (item->first == "linestyle")
        {
            histo->linestyle = item->second;
        }
        else if (item->first == "linewidth")
        {
            histo->linewidth = item->second;
        }
        else if (item->first == "color")
        {
            histo->color = item->second;
        }
        else if (item->first == "histtype")
        {
            histo->histtype = item->second;
        }
        else
        {
            std::cerr<<"Warning: attribute not supported"<<std::endl;
        }
    }

    plots.push_back(std::move(histo));

    return *this;
}

Axes & Axes::step(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::map<std::string, std::string> &options)
{
    if (xdata.size() != ydata.size())
    {
        std::cerr<<"Error: xdata and ydata size not equal"<<std::endl;
        return *this;
    }

    std::unique_ptr<PStep> step(new PStep);
    step->axesid = id;
    step->id = ++cnt;
    step->xdata = xdata;
    step->ydata = ydata;

    for (auto item=options.begin(); item!=options.end(); ++item)
    {
        if (item->first == "linestyle")
        {
            step->linestyle = item->second;
        }
        else if (item->first == "linewidth")
        {
            step->linewidth = item->second;
        }
        else if (item->first == "color")
        {
            step->color = item->second;
        }
        else if (item->first == "where")
        {
            step->where = item->second;
        }
        else
        {
            std::cerr<<"Warning: attribute not supported"<<std::endl;
        }
    }

    plots.push_back(std::move(step));

    return *this;
}

Axes & Axes::autoscale(bool enable, std::string axis, bool tight)
{
    if (enable)
    {
        if (tight)
        {
            if (axis=="both")
            {
                _autoscalex = true;
                _autoscaley = true;
            }
            else if (axis=="x")
            {
                _autoscalex = true;
            }
            else if (axis=="y")
            {
                _autoscaley = true;
            }
        }
        else
        {
            if (axis=="both")
            {
                _autoscalex = false;
                _autoscaley = false;
            }
            else if (axis=="x")
            {
                _autoscalex = false;
            }
            else if (axis=="y")
            {
                _autoscaley = false;
            }
        }
    }

    for (auto p=plots.begin(); p!=plots.end(); ++p)
    {
        (*p)->autoscale(_autoscalex, _autoscaley);
    }

    return *this;
}

Axes & Axes::draw()
{
    cpgsvp(_left, _right, _bottom, _top);

    if (_xmin == std::numeric_limits<float>::max())
    {
        _xmin = 1.;
    }
    if (_xmax == std::numeric_limits<float>::min())
    {
        _xmax = 10.;
    }
    if (_ymin == std::numeric_limits<float>::max())
    {
        _ymin = 1.;
    }
    if (_ymax == std::numeric_limits<float>::min())
    {
        _ymax = 10.;
    }

    float xmin, xmax, ymin, ymax;
    /* plots */
    if (_logx && !_logy)
    {
        xmin = std::log10(_xmin);
        xmax = std::log10(_xmax);
        ymin = _ymin;
        ymax = _ymax;
    }
    else if (!_logx && _logy)
    {
        xmin = _xmin;
        xmax = _xmax;
        ymin = std::log10(_ymin);
        ymax = std::log10(_ymax);
    }
    else if (_logx && _logy)
    {
        xmin = std::log10(_xmin);
        xmax = std::log10(_xmax);
        ymin = std::log10(_ymin);
        ymax = std::log10(_ymax);
    }
    else
    {
        xmin = _xmin;
        xmax = _xmax;
        ymin = _ymin;
        ymax = _ymax;
    }

    cpgswin(xmin, xmax, ymin, ymax);

    for (auto p=plots.begin(); p!=plots.end(); ++p)
    {
        (*p)->draw(_logx, _logy);
    }

    cpgsave();

    /* plot frame and ticks*/
    std::string xopt;
    std::string yopt;
    get_opt(xopt, yopt);
    cpgsch(get_height(_fontsize_ticklabel));
    cpgbox(xopt.c_str(), 0., 0, yopt.c_str(), 0., 0);

    /* plot labels */
    cpgsch(get_height(_fontsize_label));
    cpglab(_xlabel.c_str(), _ylabel.c_str(), _title.c_str());

    cpgunsa();

    return *this;
}

#ifdef HAVE_LIBCFITSIO
Axes & Axes::writehdu(fitsfile *fptr)
{
    int status = 0;
    /* write Axes hdu */
    fits_movnam_hdu(fptr, BINARY_TBL, "AXES", 0, &status);

    /* write data */
    if (status == BAD_HDU_NUM)
    {
        status = 0;
        char extname[] = "AXES";
        char *ttype[] = {"ID", "CNT", "VIEW", "WIN", "FRAME", "LABEL", "DIRECT", "LABELROT", "TICKS", "GRID", "LOG", "AXIS", "AUTOSCL", "LIMITS", "XLABEL", "YLABEL", "TITLE", "FSLAB", "FSTKLAB"};
        char *tform[] = {"1J", "1J", "4E", "4E", "4L", "4L", "1L", "1L", "2L", "2L", "2L", "1L", "2L", "2L", "128A", "128A", "128A", "128A", "128A"};
        char *tunit[] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
        fits_create_tbl(fptr, BINARY_TBL, 0, 19, ttype, tform, tunit, extname, &status);
    }

    long int nrows = 0;
    fits_get_num_rows(fptr, &nrows, &status);

    int colnum = 0;
    float array_float[4];
    int array_int[4];
    char array_logical[4];
    char array_char[128];

    array_int[0] =id;
    //ID
    fits_write_col(fptr, TINT, ++colnum, nrows+1, 1, 1, array_int, &status);

    array_int[0] = cnt;
    //CNT
    fits_write_col(fptr, TINT, ++colnum, nrows+1, 1, 1, array_int, &status);

    array_float[0] = _left;
    array_float[1] = _right;
    array_float[2] = _bottom;
    array_float[3] = _top;
    //VIEW
    fits_write_col(fptr, TFLOAT, ++colnum, nrows+1, 1, 4, array_float, &status);

    array_float[0] = _xmin;
    array_float[1] = _xmax;
    array_float[2] = _ymin;
    array_float[3] = _ymax;
    //WIN
    fits_write_col(fptr, TFLOAT, ++colnum, nrows+1, 1, 4, array_float, &status);

    array_logical[0] = _frameleft;
    array_logical[1] = _frameright;
    array_logical[2] = _framebottom;
    array_logical[3] = _frametop;
    //FRAME
    fits_write_col(fptr, TLOGICAL, ++colnum, nrows+1, 1, 4, array_logical, &status);

    array_logical[0] = _labelleft;
    array_logical[1] = _labelright;
    array_logical[2] = _labelbottom;
    array_logical[3] = _labeltop;
    //LABEL
    fits_write_col(fptr, TLOGICAL, ++colnum, nrows+1, 1, 4, array_logical, &status);

    array_logical[0] = _directionout;
    //DIRECT
    fits_write_col(fptr, TLOGICAL, ++colnum, nrows+1, 1, 1, array_logical, &status);

    array_logical[0] = _labelrotation;
    //LABELROT
    fits_write_col(fptr, TLOGICAL, ++colnum, nrows+1, 1, 1, array_logical, &status);

    array_logical[0] = _minorticks;
    array_logical[1] = _majorticks;
    //TICKS
    fits_write_col(fptr, TLOGICAL, ++colnum, nrows+1, 1, 2, array_logical, &status);

    array_logical[0] = _gridvertical;
    array_logical[1] = _gridhorizontal;
    //GRID
    fits_write_col(fptr, TLOGICAL, ++colnum, nrows+1, 1, 2, array_logical, &status);

    array_logical[0] = _logx;
    array_logical[1] = _logy;
    //LOG
    fits_write_col(fptr, TLOGICAL, ++colnum, nrows+1, 1, 2, array_logical, &status);

    array_logical[0] = _axis;
    //AXIS
    fits_write_col(fptr, TLOGICAL, ++colnum, nrows+1, 1, 1, array_logical, &status);

    array_logical[0] = _autoscalex;
    array_logical[1] = _autoscaley;
    //AUTOSCL
    fits_write_col(fptr, TLOGICAL, ++colnum, nrows+1, 1, 2, array_logical, &status);

    array_logical[0] = _xlimits;
    array_logical[1] = _ylimits;
    //LIMITS
    fits_write_col(fptr, TLOGICAL, ++colnum, nrows+1, 1, 2, array_logical, &status);

    //XLABEL
    fits_write_col(fptr, TSTRING, ++colnum, nrows+1, 1, 1, &_xlabel, &status);

    //YLABEL
    fits_write_col(fptr, TSTRING, ++colnum, nrows+1, 1, 1, &_ylabel, &status);

    //TITLE
    fits_write_col(fptr, TSTRING, ++colnum, nrows+1, 1, 1, &_title, &status);

    //FSLAB
    fits_write_col(fptr, TSTRING, ++colnum, nrows+1, 1, 1, &_fontsize_label, &status);

    //FSTKLAB
    fits_write_col(fptr, TSTRING, ++colnum, nrows+1, 1, 1, &_fontsize_ticklabel, &status);

    fits_report_error(stderr, status);

    return *this;
}
#endif

void Axes::get_opt(std::string &xopt, std::string &yopt)
{
    if (_axis) xopt += "A";
    if (_axis) yopt += "A";

    if (_framebottom) xopt += "B";
    if (_frametop) xopt += "C";
    if (_frameleft) yopt += "B";
    if (_frameright) yopt += "C";

    if (_gridvertical) xopt += "G";
    if (_gridhorizontal) yopt += "G";

    if (_directionout)
    {
        xopt += "I";
        yopt += "I";
    }

    if (_logx) xopt += "L";
    if (_logy) yopt += "L";

    if (_majorticks)
    {
        xopt += "T";
        yopt += "T";
    }

    if (_minorticks)
    {
        xopt += "S";
        yopt += "S";
    }

    if (_labelbottom) xopt += "N";
    if (_labelleft) yopt += "N";
    if (_labeltop) xopt += "M";
    if (_labelright) yopt += "M";

    if (_labelrotation) yopt += "V";
}

void Axes::get_win()
{
    if (plots.empty())
    {
        if (!_xlimits)
        {
            _xmin = 0.;
            _xmax = 1.;
        }

        if (!_ylimits)
        {
            _ymin = 0.;
            _ymax = 1.;
        }
    }

    for (auto p=plots.begin(); p!=plots.end(); ++p)
    {
        float xmin = std::numeric_limits<float>::max();
        float xmax = std::numeric_limits<float>::min();
        float ymin = std::numeric_limits<float>::max();
        float ymax = std::numeric_limits<float>::min();
        
        (*p)->get_win(xmin, xmax, ymin, ymax);

        if (!(*p)->_autoscalex)
        {
            if (!_logx)
            {
                float dx = xmax-xmin;
                xmin -= dx/20.;
                xmax += dx/20.;
            }
            else
            {
                float dxlog = std::log10(xmax)-std::log10(xmin);
                float xminlog = std::log10(xmin);
                float xmaxlog = std::log10(xmax);
                xmin = std::pow(10., xminlog-dxlog/20.);
                xmax = std::pow(10., xmaxlog+dxlog/20.);
            }
        }

        if (!(*p)->_autoscaley)
        {
            if (!_logy)
            {
                float dy = ymax-ymin;
                ymin -= dy/20.;
                ymax += dy/20.;
            }
            else
            {
                float dylog = std::log10(ymax)-std::log10(ymin);
                float yminlog = std::log10(ymin);
                float ymaxlog = std::log10(ymax);
                ymin = std::pow(10., yminlog-dylog/20.);
                ymax = std::pow(10., ymaxlog+dylog/20.);
            }
        }

        if (!_xlimits)
        {
            _xmin = xmin < _xmin ? xmin : _xmin;
            _xmax = xmax > _xmax ? xmax : _xmax;
        }
        if (!_ylimits)
        {
            _ymin = ymin < _ymin ? ymin : _ymin;
            _ymax = ymax > _ymax ? ymax : _ymax;
        }
    }
}

/*================== PLine =================*/
PLine & PLine::draw(bool logx, bool logy)
{
    cpgsave();

    /* set linestyle */
    cpgsls(get_linestyle(linestyle));

    /* set linewidth */
    cpgslw(get_linewidth(linewidth));

    /* set color */
    cpgsci(get_color(color));

    if (logx && !logy)
    {
        std::vector<float> xdatalog;
        std::transform(xdata.begin(), xdata.end(), std::back_inserter(xdatalog), [](float x){return std::log10(x);});

        cpgline(xdata.size(), xdatalog.data(), ydata.data());
    }
    else if (!logx && logy)
    {
        std::vector<float> ydatalog;
        std::transform(ydata.begin(), ydata.end(), std::back_inserter(ydatalog), [](float x){return std::log10(x);});

        cpgline(xdata.size(), xdata.data(), ydatalog.data());
    }
    else if (logx && logy)
    {
        std::vector<float> xdatalog, ydatalog;
        std::transform(xdata.begin(), xdata.end(), std::back_inserter(xdatalog), [](float x){return std::log10(x);});
        std::transform(ydata.begin(), ydata.end(), std::back_inserter(ydatalog), [](float x){return std::log10(x);});

        cpgline(xdata.size(), xdatalog.data(), ydatalog.data());
    }
    else
    {
        cpgline(xdata.size(), xdata.data(), ydata.data());
    }

    cpgunsa();

    return *this;
}

PLine * PLine::clone()
{
    PLine *line = new PLine;

    *line = *this;

    return line;
}

void PLine::get_win(float &xmin, float &xmax, float &ymin, float &ymax)
{
    xmin = *(std::min_element(xdata.begin(), xdata.end()));
    xmax = *(std::max_element(xdata.begin(), xdata.end()));
    ymin = *(std::min_element(ydata.begin(), ydata.end()));
    ymax = *(std::max_element(ydata.begin(), ydata.end()));
}

#ifdef HAVE_LIBCFITSIO
PLine & PLine::writehdu(fitsfile *fptr)
{
    int status = 0;
    /* write XYPlot hdu */

    /* write data */
    char extname[] = "LINE";
    char *ttype[] = {"XDATA", "YDATA"};
    char *tform[] = {"E", "E"};
    char *tunit[] = {"", ""};
    fits_create_tbl(fptr, BINARY_TBL, 1, 2, ttype, tform, tunit, extname, &status);

    int colnum = 1;
    fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
    fits_modify_vector_len(fptr, colnum, xdata.size(), &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, xdata.size(), xdata.data(), &status);

    fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
    fits_modify_vector_len(fptr, colnum, ydata.size(), &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, ydata.size(), ydata.data(), &status);

    /* write header */
    char value[FLEN_VALUE] = "Line";
    fits_update_key(fptr, TSTRING, "PLOTTYPE", value, "plot type", &status);
    fits_update_key(fptr, TSTRING, "PLOTNAME", const_cast<char *>(plotname.c_str()), "plot name", &status);
    fits_update_key(fptr, TINT, "AXES", &axesid, "axes id", &status);
    fits_update_key(fptr, TINT, "ID", &id, "plot id", &status);
    fits_update_key(fptr, TSTRING, "LINESTY", const_cast<char *>(linestyle.c_str()), "line style [-, --, ., -., -...]", &status);
    fits_update_key(fptr, TSTRING, "LINEWID", const_cast<char *>(linewidth.c_str()), "linewidth", &status);
    fits_update_key(fptr, TSTRING, "COLOR", const_cast<char *>(color.c_str()), "color", &status);

	fits_report_error(stderr, status);

    return *this;
}
#endif

/*================== PAxvline =================*/
PAxvline & PAxvline::draw(bool logx, bool logy)
{
    cpgsave();

    /* set linestyle */
    cpgsls(get_linestyle(linestyle));

    /* set linewidth */
    cpgslw(get_linewidth(linewidth));

    /* set color */
    cpgsci(get_color(color));

    float xmin_win, xmax_win, ymin_win, ymax_win;
    cpgqwin(&xmin_win, &xmax_win, &ymin_win, &ymax_win);

    float ymin = ymin_win+(_ymin-0)*(ymax_win-ymin_win);
    float ymax = ymin_win+(_ymax-0)*(ymax_win-ymin_win);

    if (logx && !logy)
    {
        float xdata[2] = {std::log10(x), std::log10(x)};
        float ydata[2] = {ymin, ymax};
        cpgline(2, xdata, ydata);
    }
    else if (!logx && logy)
    {
        float xdata[2] = {x, x};
        float ydata[2] = {ymin, ymax};
        cpgline(2, xdata, ydata);
    }
    else if (logx && logy)
    {
        float xdata[2] = {std::log10(x), std::log10(x)};
        float ydata[2] = {ymin, ymax};
        cpgline(2, xdata, ydata);
    }
    else
    {
        float xdata[2] = {x, x};
        float ydata[2] = {ymin, ymax};
        cpgline(2, xdata, ydata);
    }

    cpgunsa();

    return *this;
}

PAxvline * PAxvline::clone()
{
    PAxvline *line = new PAxvline;

    *line = *this;

    return line;
}

void PAxvline::get_win(float &xmin, float &xmax, float &ymin, float &ymax)
{
    xmin = x;
    xmax = x;
}

PAxvline & PAxvline::writehdu(fitsfile *fptr)
{
    int status = 0;
    /* write XYPlot hdu */

    /* write data */
    char extname[] = "AXVLINE";
    char *ttype[] = {"XDATA", "YDATA"};
    char *tform[] = {"1E", "2E"};
    char *tunit[] = {"", ""};
    fits_create_tbl(fptr, BINARY_TBL, 1, 2, ttype, tform, tunit, extname, &status);

    int colnum = 1;
    fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, 1, &x, &status);

    fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
    float array_float[2] = {_ymin, _ymax};
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, 2, array_float, &status);

    /* write header */
    char value[FLEN_VALUE] = "Axvline";
    fits_update_key(fptr, TSTRING, "PLOTTYPE", value, "plot type", &status);
    fits_update_key(fptr, TSTRING, "PLOTNAME", const_cast<char *>(plotname.c_str()), "plot name", &status);
    fits_update_key(fptr, TINT, "AXES", &axesid, "axes id", &status);
    fits_update_key(fptr, TINT, "ID", &id, "plot id", &status);
    fits_update_key(fptr, TSTRING, "LINESTY", const_cast<char *>(linestyle.c_str()), "line style [-, --, ., -., -...]", &status);
    fits_update_key(fptr, TSTRING, "LINEWID", const_cast<char *>(linewidth.c_str()), "linewidth", &status);
    fits_update_key(fptr, TSTRING, "COLOR", const_cast<char *>(color.c_str()), "color", &status);

	fits_report_error(stderr, status);

    return *this;
}

/*================== PAxhline =================*/
PAxhline & PAxhline::draw(bool logx, bool logy)
{
    cpgsave();

    /* set linestyle */
    cpgsls(get_linestyle(linestyle));

    /* set linewidth */
    cpgslw(get_linewidth(linewidth));

    /* set color */
    cpgsci(get_color(color));

    float xmin_win, xmax_win, ymin_win, ymax_win;
    cpgqwin(&xmin_win, &xmax_win, &ymin_win, &ymax_win);

    float xmin = xmin_win+(_xmin-0)*(xmax_win-xmin_win);
    float xmax = xmin_win+(_xmax-0)*(xmax_win-xmin_win);

    if (logy && !logx)
    {
        float ydata[2] = {std::log10(y), std::log10(y)};
        float xdata[2] = {xmin, xmax};
        cpgline(2, xdata, ydata);
    }
    else if (!logy && logx)
    {
        float ydata[2] = {y, y};
        float xdata[2] = {xmin, xmax};
        cpgline(2, xdata, ydata);
    }
    else if (logx && logy)
    {
        float ydata[2] = {std::log10(y), std::log10(y)};
        float xdata[2] = {xmin, xmax};
        cpgline(2, xdata, ydata);
    }
    else
    {
        float ydata[2] = {y, y};
        float xdata[2] = {xmin, xmax};
        cpgline(2, xdata, ydata);
    }

    cpgunsa();

    return *this;
}

PAxhline * PAxhline::clone()
{
    PAxhline *line = new PAxhline;

    *line = *this;

    return line;
}

void PAxhline::get_win(float &xmin, float &xmax, float &ymin, float &ymax)
{
    ymin = y;
    ymax = y;
}

#ifdef HAVE_LIBCFITSIO
PAxhline & PAxhline::writehdu(fitsfile *fptr)
{
    int status = 0;
    /* write XYPlot hdu */

    /* write data */
    char extname[] = "AXHLINE";
    char *ttype[] = {"XDATA", "YDATA"};
    char *tform[] = {"2E", "1E"};
    char *tunit[] = {"", ""};
    fits_create_tbl(fptr, BINARY_TBL, 1, 2, ttype, tform, tunit, extname, &status);

    int colnum = 1;
    fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
    float array_float[2] = {_xmin, _xmax};
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, 2, array_float, &status);

    fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, 1, &y, &status);

    /* write header */
    char value[FLEN_VALUE] = "Axhline";
    fits_update_key(fptr, TSTRING, "PLOTTYPE", value, "plot type", &status);
    fits_update_key(fptr, TSTRING, "PLOTNAME", const_cast<char *>(plotname.c_str()), "plot name", &status);
    fits_update_key(fptr, TINT, "AXES", &axesid, "axes id", &status);
    fits_update_key(fptr, TINT, "ID", &id, "plot id", &status);
    fits_update_key(fptr, TSTRING, "LINESTY", const_cast<char *>(linestyle.c_str()), "line style [-, --, ., -., -...]", &status);
    fits_update_key(fptr, TSTRING, "LINEWID", const_cast<char *>(linewidth.c_str()), "linewidth", &status);
    fits_update_key(fptr, TSTRING, "COLOR", const_cast<char *>(color.c_str()), "color", &status);

	fits_report_error(stderr, status);

    return *this;
}
#endif

/*================== PAxvspan =================*/
PAxvspan & PAxvspan::draw(bool logx, bool logy)
{
    cpgsave();

    /* set linestyle */
    cpgsls(get_linestyle(linestyle));

    /* set linewidth */
    cpgslw(get_linewidth(linewidth));

    /* set color */
    cpgsci(get_color(color));

    /* set filled */
    cpgsfs(get_filled(filled));

    float xmin_win, xmax_win, ymin_win, ymax_win;
    cpgqwin(&xmin_win, &xmax_win, &ymin_win, &ymax_win);

    float ymin = ymin_win+(_ymin-0)*(ymax_win-ymin_win);
    float ymax = ymin_win+(_ymax-0)*(ymax_win-ymin_win);

    if (logx && !logy)
    {
       cpgrect(std::log10(_xmin), std::log10(_xmax), ymin, ymax);
    }
    else if (!logx && logy)
    {
        cpgrect(_xmin, _xmax, ymin, ymax);
    }
    else if (logx && logy)
    {
        cpgrect(std::log10(_xmin), std::log10(_xmax), ymin, ymax);
    }
    else
    {
        cpgrect(_xmin, _xmax, ymin, ymax);
    }

    cpgunsa();

    return *this;
}

PAxvspan * PAxvspan::clone()
{
    PAxvspan *span = new PAxvspan;

    *span = *this;

    return span;
}

void PAxvspan::get_win(float &xmin, float &xmax, float &ymin, float &ymax)
{
    xmin = _xmin;
    xmax = _xmax;
}

#ifdef HAVE_LIBCFITSIO
PAxvspan & PAxvspan::writehdu(fitsfile *fptr)
{
    int status = 0;
    /* write XYPlot hdu */

    /* write data */
    char extname[] = "AXVSPAN";
    char *ttype[] = {"XDATA", "YDATA"};
    char *tform[] = {"2E", "2E"};
    char *tunit[] = {"", ""};
    fits_create_tbl(fptr, BINARY_TBL, 1, 2, ttype, tform, tunit, extname, &status);

    int colnum = 1;
    fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
    float array_float[2] = {_xmin, _xmax};
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, 2, array_float, &status);

    fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
    array_float[0] = _ymin;
    array_float[1] = _ymax;
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, 2, array_float, &status);

    /* write header */
    char value[FLEN_VALUE] = "Axvspan";
    fits_update_key(fptr, TSTRING, "PLOTTYPE", value, "plot type", &status);
    fits_update_key(fptr, TSTRING, "PLOTNAME", const_cast<char *>(plotname.c_str()), "plot name", &status);
    fits_update_key(fptr, TINT, "AXES", &axesid, "axes id", &status);
    fits_update_key(fptr, TINT, "ID", &id, "plot id", &status);
    fits_update_key(fptr, TSTRING, "LINESTY", const_cast<char *>(linestyle.c_str()), "line style [-, --, ., -., -...]", &status);
    fits_update_key(fptr, TSTRING, "LINEWID", const_cast<char *>(linewidth.c_str()), "linewidth", &status);
    fits_update_key(fptr, TSTRING, "COLOR", const_cast<char *>(color.c_str()), "color", &status);
    fits_update_key(fptr, TSTRING, "FILLED", const_cast<char *>(filled.c_str()), "filled [true, false]", &status);

	fits_report_error(stderr, status);

    return *this;
}
#endif

/*================== PAxhspan =================*/
PAxhspan & PAxhspan::draw(bool logx, bool logy)
{
    cpgsave();

    /* set linestyle */
    cpgsls(get_linestyle(linestyle));

    /* set linewidth */
    cpgslw(get_linewidth(linewidth));

    /* set color */
    cpgsci(get_color(color));

    /* set filled */
    cpgsfs(get_filled(filled));

    float xmin_win, xmax_win, ymin_win, ymax_win;
    cpgqwin(&xmin_win, &xmax_win, &ymin_win, &ymax_win);

    float xmin = xmin_win+(_xmin-0)*(xmax_win-xmin_win);
    float xmax = xmin_win+(_xmax-0)*(xmax_win-xmin_win);

    if (logx && !logy)
    {
       cpgrect(xmin, xmax, _ymin, _ymax);
    }
    else if (!logx && logy)
    {
        cpgrect(xmin, xmax, std::log10(_ymin), std::log10(_ymax));
    }
    else if (logx && logy)
    {
        cpgrect(xmin, xmax, std::log10(_ymin), std::log10(_ymax));
    }
    else
    {
        cpgrect(xmin, xmax, _ymin, _ymax);
    }

    cpgunsa();

    return *this;
}

PAxhspan * PAxhspan::clone()
{
    PAxhspan *span = new PAxhspan;

    *span = *this;

    return span;
}

void PAxhspan::get_win(float &xmin, float &xmax, float &ymin, float &ymax)
{
    ymin = _ymin;
    ymax = _ymax;
}

#ifdef HAVE_LIBCFITSIO
PAxhspan & PAxhspan::writehdu(fitsfile *fptr)
{
    int status = 0;
    /* write XYPlot hdu */

    /* write data */
    char extname[] = "AXHSPAN";
    char *ttype[] = {"XDATA", "YDATA"};
    char *tform[] = {"2E", "2E"};
    char *tunit[] = {"", ""};
    fits_create_tbl(fptr, BINARY_TBL, 1, 2, ttype, tform, tunit, extname, &status);

    int colnum = 1;
    fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
    float array_float[2] = {_xmin, _xmax};
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, 2, array_float, &status);

    fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
    array_float[0] = _ymin;
    array_float[1] = _ymax;
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, 2, array_float, &status);

    /* write header */
    char value[FLEN_VALUE] = "Axhspan";
    fits_update_key(fptr, TSTRING, "PLOTTYPE", value, "plot type", &status);
    fits_update_key(fptr, TSTRING, "PLOTNAME", const_cast<char *>(plotname.c_str()), "plot name", &status);
    fits_update_key(fptr, TINT, "AXES", &axesid, "axes id", &status);
    fits_update_key(fptr, TINT, "ID", &id, "plot id", &status);
    fits_update_key(fptr, TSTRING, "LINESTY", const_cast<char *>(linestyle.c_str()), "line style [-, --, ., -., -...]", &status);
    fits_update_key(fptr, TSTRING, "LINEWID", const_cast<char *>(linewidth.c_str()), "linewidth", &status);
    fits_update_key(fptr, TSTRING, "COLOR", const_cast<char *>(color.c_str()), "color", &status);
    fits_update_key(fptr, TSTRING, "FILLED", const_cast<char *>(filled.c_str()), "filled [true, false]", &status);

	fits_report_error(stderr, status);

    return *this; 
}
#endif

/*================== PScatter =================*/
PScatter & PScatter::draw(bool logx, bool logy)
{
    cpgsave();

    /* set markersize */
    cpgsch(get_height(markersize));

    /* set color */
    cpgsci(get_color(color));

    if (logx && !logy)
    {
        std::vector<float> xdatalog;
        std::transform(xdata.begin(), xdata.end(), std::back_inserter(xdatalog), [](float x){return std::log10(x);});
        
        cpgpt(xdata.size(), xdatalog.data(), ydata.data(), get_symbol(marker));
    }
    else if (!logx && logy)
    {
        std::vector<float> ydatalog;
        std::transform(ydata.begin(), ydata.end(), std::back_inserter(ydatalog), [](float x){return std::log10(x);});

        cpgpt(xdata.size(), xdata.data(), ydatalog.data(), get_symbol(marker));
    }
    else if (logx && logy)
    {
        std::vector<float> xdatalog, ydatalog;
        std::transform(xdata.begin(), xdata.end(), std::back_inserter(xdatalog), [](float x){return std::log10(x);});
        std::transform(ydata.begin(), ydata.end(), std::back_inserter(ydatalog), [](float x){return std::log10(x);});

        cpgpt(xdata.size(), xdatalog.data(), ydatalog.data(), get_symbol(marker));
    }
    else
    {
        cpgpt(xdata.size(), xdata.data(), ydata.data(), get_symbol(marker));
    }

    cpgunsa();

    return *this;
}

PScatter * PScatter::clone()
{
    PScatter *scatter = new PScatter;

    *scatter = *this;

    return scatter;
}

void PScatter::get_win(float &xmin, float &xmax, float &ymin, float &ymax)
{
    xmin = *(std::min_element(xdata.begin(), xdata.end()));
    xmax = *(std::max_element(xdata.begin(), xdata.end()));
    ymin = *(std::min_element(ydata.begin(), ydata.end()));
    ymax = *(std::max_element(ydata.begin(), ydata.end()));
}

#ifdef HAVE_LIBCFITSIO
PScatter & PScatter::writehdu(fitsfile *fptr)
{
    int status = 0;
    /* write XYPlot hdu */

    /* write data */
    char extname[] = "SCATTER";
    char *ttype[] = {"XDATA", "YDATA"};
    char *tform[] = {"E", "E"};
    char *tunit[] = {"", ""};
    fits_create_tbl(fptr, BINARY_TBL, 1, 2, ttype, tform, tunit, extname, &status);

    int colnum = 1;
    fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
    fits_modify_vector_len(fptr, colnum, xdata.size(), &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, xdata.size(), xdata.data(), &status);

    fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
    fits_modify_vector_len(fptr, colnum, ydata.size(), &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, ydata.size(), ydata.data(), &status);

    /* write header */
    char value[FLEN_VALUE] = "Scatter";
    fits_update_key(fptr, TSTRING, "PLOTTYPE", value, "plot type", &status);
    fits_update_key(fptr, TSTRING, "PLOTNAME", const_cast<char *>(plotname.c_str()), "plot name", &status);
    fits_update_key(fptr, TINT, "AXES", &axesid, "axes id", &status);
    fits_update_key(fptr, TINT, "ID", &id, "plot id", &status);
    fits_update_key(fptr, TSTRING, "MARKER", const_cast<char *>(marker.c_str()), "marker type [0-31]", &status);
    fits_update_key(fptr, TSTRING, "MKSIZE", const_cast<char *>(markersize.c_str()), "marker size", &status);
    fits_update_key(fptr, TSTRING, "COLOR", const_cast<char *>(color.c_str()), "color", &status);

	fits_report_error(stderr, status);

    return *this;
}
#endif

/*================== PPoint =================*/
PPoint & PPoint::draw(bool logx, bool logy)
{
    cpgsave();

    /* set markersize */
    cpgsch(get_height(markersize));

    /* set color */
    cpgsci(get_color(color));

    if (logx && !logy)
        cpgpt1(std::log10(x), y, get_symbol(marker));
    else if (!logx && logy)
        cpgpt1(x, std::log10(y), get_symbol(marker));
    else if (logx && logy)
        cpgpt1(std::log10(x), std::log10(y), get_symbol(marker));
    else
        cpgpt1(x, y, get_symbol(marker));

    cpgunsa();

    return *this;
}

PPoint * PPoint::clone()
{
    PPoint *point = new PPoint;

    *point = *this;

    return point;
}

void PPoint::get_win(float &xmin, float &xmax, float &ymin, float &ymax)
{
    xmin = x;
    xmax = x;
    ymin = y;
    ymax = y;
}

#ifdef HAVE_LIBCFITSIO
PPoint & PPoint::writehdu(fitsfile *fptr)
{
    int status = 0;
    /* write XYPlot hdu */

    /* write data */
    char extname[] = "POINT";
    char *ttype[] = {"XDATA", "YDATA"};
    char *tform[] = {"1E", "1E"};
    char *tunit[] = {"", ""};
    fits_create_tbl(fptr, BINARY_TBL, 1, 2, ttype, tform, tunit, extname, &status);

    int colnum = 1;
    fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, 1, &x, &status);

    fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, 1, &y, &status);

    /* write header */
    char value[FLEN_VALUE] = "Point";
    fits_update_key(fptr, TSTRING, "PLOTTYPE", value, "plot type", &status);
    fits_update_key(fptr, TSTRING, "PLOTNAME", const_cast<char *>(plotname.c_str()), "plot name", &status);
    fits_update_key(fptr, TINT, "AXES", &axesid, "axes id", &status);
    fits_update_key(fptr, TINT, "ID", &id, "plot id", &status);
    fits_update_key(fptr, TSTRING, "MARKER", const_cast<char *>(marker.c_str()), "marker type [0-31]", &status);
    fits_update_key(fptr, TSTRING, "MKSIZE", const_cast<char *>(markersize.c_str()), "marker size", &status);
    fits_update_key(fptr, TSTRING, "COLOR", const_cast<char *>(color.c_str()), "color", &status);

	fits_report_error(stderr, status);

    return *this;
}
#endif

/*================== PPoint =================*/
PPcolor & PPcolor::draw(bool logx, bool logy)
{
    float xmin = xdata.front();
    float xmax = xdata.back();
    float ymin = ydata.front();
    float ymax = ydata.back();
    float zmin = *(std::min_element(zdata.begin(), zdata.end()));
    float zmax = *(std::max_element(zdata.begin(), zdata.end()));

    int idim = xdata.size();
    int jdim = ydata.size();

    int i1 = 1, i2 = idim;
    int j1 = 1, j2 = jdim;

    float tr[6];
    tr[0] = (xmin*i2-xmax*i1)/(i2-i1);
    tr[1] = (xmax-xmin)/(i2-i1);
    tr[2] = 0.;
    tr[3] = (ymin*j2-ymax*j1)/(j2-j1);
    tr[4] = 0.;
    tr[5] = (ymax-ymin)/(j2-j1);

    set_cmap(cmap, cm_data);
    cpgimag(zdata.data(), idim, jdim, i1, i2, j1, j2, zmin, zmax, tr);

    return *this;
}

PPcolor * PPcolor::clone()
{
    PPcolor *image = new PPcolor;

    *image = *this;

    return image;
}

void PPcolor::get_win(float &xmin, float &xmax, float &ymin, float &ymax)
{
    xmin = *(std::min_element(xdata.begin(), xdata.end()));
    xmax = *(std::max_element(xdata.begin(), xdata.end()));
    ymin = *(std::min_element(ydata.begin(), ydata.end()));
    ymax = *(std::max_element(ydata.begin(), ydata.end()));
}

#ifdef HAVE_LIBCFITSIO
PPcolor & PPcolor::writehdu(fitsfile *fptr)
{
    int status = 0;
    /* write XYPlot hdu */

    /* write data */
    char extname[] = "PCOLOR";
    char *ttype[] = {"XDATA", "YDATA", "ZDATA"};
    char *tform[] = {"E", "E", "E"};
    char *tunit[] = {"", "", ""};
    fits_create_tbl(fptr, BINARY_TBL, 1, 3, ttype, tform, tunit, extname, &status);

    int colnum = 1;
    fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
    fits_modify_vector_len(fptr, colnum, xdata.size(), &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, xdata.size(), xdata.data(), &status);

    fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
    fits_modify_vector_len(fptr, colnum, ydata.size(), &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, ydata.size(), ydata.data(), &status);

    fits_get_colnum(fptr, CASEINSEN, "ZDATA", &colnum, &status);
    fits_modify_vector_len(fptr, colnum, zdata.size(), &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, zdata.size(), zdata.data(), &status);

    /* write header */
    char value[FLEN_VALUE] = "Pcolor";
    fits_update_key(fptr, TSTRING, "PLOTTYPE", value, "plot type", &status);
    fits_update_key(fptr, TSTRING, "PLOTNAME", const_cast<char *>(plotname.c_str()), "plot name", &status);
    fits_update_key(fptr, TINT, "AXES", &axesid, "axes id", &status);
    fits_update_key(fptr, TINT, "ID", &id, "plot id", &status);
    fits_update_key(fptr, TSTRING, "CMAP", const_cast<char *>(cmap.c_str()), "color", &status);

	fits_report_error(stderr, status);

    return *this;    
}
#endif

/*================== PErrorbar =================*/
PErrorbar & PErrorbar::draw(bool logx, bool logy)
{
    cpgsave();

    cpgslw(get_linewidth(linewidth));
    cpgsci(get_color(color));

    std::vector<float> xdatalog, ydatalog;
    std::vector<std::vector<float>> xerrlog, yerrlog;
    
    std::transform(xdata.begin(), xdata.end(), std::back_inserter(xdatalog), [](float x){return std::log10(x);});
    std::transform(ydata.begin(), ydata.end(), std::back_inserter(ydatalog), [](float x){return std::log10(x);});

    if (!xerr.empty())
    {
        xerrlog.resize(2);
        std::transform(xdata.begin(), xdata.end(), xerr.front().begin(), std::back_inserter(xerrlog[0]), [](float v, float err){return std::log10(v)-std::log10(v-err);});
        std::transform(xdata.begin(), xdata.end(), xerr.back().begin(), std::back_inserter(xerrlog[1]), [](float v, float err){return std::log10(v+err)-std::log10(v);});
    }
    if (!yerr.empty())
    {
        yerrlog.resize(2);
        std::transform(ydata.begin(), ydata.end(), yerr.front().begin(), std::back_inserter(yerrlog[0]), [](float v, float err){return std::log10(v)-std::log10(v-err);});
        std::transform(ydata.begin(), ydata.end(), yerr.back().begin(), std::back_inserter(yerrlog[1]), [](float v, float err){return std::log10(v+err)-std::log10(v);});
    }

    if (xerr.size() >= 1)
    {
        if (logx && !logy)
        {
            if (xuplims != "False" and xuplims != "" and xuplims != "none" and xuplims != "None" and xuplims != "off")
                cpgerrb(1, xdata.size(), xdatalog.data(), ydata.data(), xerrlog.back().data(), stof(capsize));
            if (xlolims != "False" and xlolims != "" and xlolims != "none" and xlolims != "None" and xlolims != "off")
                cpgerrb(3, xdata.size(), xdatalog.data(), ydata.data(), xerrlog.front().data(), stof(capsize));
        }
        else if (!logx && logy)
        {
            if (xuplims != "False" and xuplims != "" and xuplims != "none" and xuplims != "None" and xuplims != "off")
                cpgerrb(1, xdata.size(), xdata.data(), ydatalog.data(), xerr.back().data(), stof(capsize));
            if (xlolims != "False" and xlolims != "" and xlolims != "none" and xlolims != "None" and xlolims != "off")
                cpgerrb(3, xdata.size(), xdata.data(), ydatalog.data(), xerr.front().data(), stof(capsize));
        }
        else if (logx && logy)
        {
            if (xuplims != "False" and xuplims != "" and xuplims != "none" and xuplims != "None" and xuplims != "off")
                cpgerrb(1, xdata.size(), xdatalog.data(), ydatalog.data(), xerrlog.back().data(), stof(capsize));
            if (xlolims != "False" and xlolims != "" and xlolims != "none" and xlolims != "None" and xlolims != "off")
                cpgerrb(3, xdata.size(), xdatalog.data(), ydatalog.data(), xerrlog.front().data(), stof(capsize));
        }
        else
        {
            if (xuplims != "False" and xuplims != "" and xuplims != "none" and xuplims != "None" and xuplims != "off")
                cpgerrb(1, xdata.size(), xdata.data(), ydata.data(), xerr.back().data(), stof(capsize));
            if (xlolims != "False" and xlolims != "" and xlolims != "none" and xlolims != "None" and xlolims != "off")
                cpgerrb(3, xdata.size(), xdata.data(), ydata.data(), xerr.front().data(), stof(capsize));
        }
    }

    if (yerr.size() >= 1)
    {
        if (logx && !logy)
        {
            if (uplims != "False" and uplims != "" and uplims != "none" and uplims != "None" and uplims != "off")
                cpgerrb(2, ydata.size(), xdatalog.data(), ydata.data(), yerr.back().data(), stof(capsize));
            if (lolims != "False" and lolims != "" and lolims != "none" and lolims != "None" and lolims != "off")
                cpgerrb(4, ydata.size(), xdatalog.data(), ydata.data(), yerr.front().data(), stof(capsize));
        }
        else if (!logx && logy)
        {
            if (uplims != "False" and uplims != "" and uplims != "none" and uplims != "None" and uplims != "off")
                cpgerrb(2, ydata.size(), xdata.data(), ydatalog.data(), yerrlog.back().data(), stof(capsize));
            if (lolims != "False" and lolims != "" and lolims != "none" and lolims != "None" and lolims != "off")
                cpgerrb(4, ydata.size(), xdata.data(), ydatalog.data(), yerrlog.front().data(), stof(capsize));
        }
        else if (logx && logy)
        {
            if (uplims != "False" and uplims != "" and uplims != "none" and uplims != "None" and uplims != "off")
                cpgerrb(2, ydata.size(), xdatalog.data(), ydatalog.data(), yerrlog.back().data(), stof(capsize));
            if (lolims != "False" and lolims != "" and lolims != "none" and lolims != "None" and lolims != "off")
                cpgerrb(4, ydata.size(), xdatalog.data(), ydatalog.data(), yerrlog.front().data(), stof(capsize));
        }
        else
        {
            if (uplims != "False" and uplims != "" and uplims != "none" and uplims != "None" and uplims != "off")
                cpgerrb(2, ydata.size(), xdata.data(), ydata.data(), yerr.back().data(), stof(capsize));
            if (lolims != "False" and lolims != "" and lolims != "none" and lolims != "None" and lolims != "off")
                cpgerrb(4, ydata.size(), xdata.data(), ydata.data(), yerr.front().data(), stof(capsize));
        }
    }

    cpgunsa();

    return *this;
}

PErrorbar * PErrorbar::clone()
{
    PErrorbar *errbar = new PErrorbar;

    *errbar = *this;

    return errbar;
}

void PErrorbar::get_win(float &xmin, float &xmax, float &ymin, float &ymax)
{
    std::vector<float> xh=xdata, xl=xdata;
    if (!xerr.empty())
    {
        std::transform(xdata.begin(), xdata.end(), xerr.back().begin(), xh.begin(), std::plus<float>());
        std::transform(xdata.begin(), xdata.end(), xerr.front().begin(), xl.begin(), std::minus<float>());
    }

    std::vector<float> yh=ydata, yl=ydata;
    if (!yerr.empty())
    {
        std::transform(ydata.begin(), ydata.end(), yerr.back().begin(), yh.begin(), std::plus<float>());
        std::transform(ydata.begin(), ydata.end(), yerr.front().begin(), yl.begin(), std::minus<float>());
    }

    xmin = *(std::min_element(xl.begin(), xl.end()));
    xmax = *(std::max_element(xh.begin(), xh.end()));
    ymin = *(std::min_element(yl.begin(), yl.end()));
    ymax = *(std::max_element(yh.begin(), yh.end()));
}

#ifdef HAVE_LIBCFITSIO
PErrorbar & PErrorbar::writehdu(fitsfile *fptr)
{
    int status = 0;
    /* write XYPlot hdu */

    /* write data */
    char extname[] = "ERRORBAR";
    char *ttype[] = {"XDATA", "YDATA", "XERRL", "XERRU", "YERRL", "YERRU"};
    char *tform[] = {"E", "E", "0E", "0E", "0E", "0E"};
    char *tunit[] = {"", "", "", "", "", ""};
    fits_create_tbl(fptr, BINARY_TBL, 1, 6, ttype, tform, tunit, extname, &status);

    int colnum = 1;
    fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
    fits_modify_vector_len(fptr, colnum, xdata.size(), &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, xdata.size(), xdata.data(), &status);

    fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
    fits_modify_vector_len(fptr, colnum, ydata.size(), &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, ydata.size(), ydata.data(), &status);

    if (!xerr.empty())
    {
        fits_get_colnum(fptr, CASEINSEN, "XERRL", &colnum, &status);
        fits_modify_vector_len(fptr, colnum, xerr.front().size(), &status);
        fits_write_col(fptr, TFLOAT, colnum, 1, 1, xerr.front().size(), xerr.front().data(), &status);

        fits_get_colnum(fptr, CASEINSEN, "XERRU", &colnum, &status);
        fits_modify_vector_len(fptr, colnum, xerr.back().size(), &status);
        fits_write_col(fptr, TFLOAT, colnum, 1, 1, xerr.back().size(), xerr.back().data(), &status);
    }

    if (!yerr.empty())
    {
        fits_get_colnum(fptr, CASEINSEN, "YERRL", &colnum, &status);
        fits_modify_vector_len(fptr, colnum, yerr.front().size(), &status);
        fits_write_col(fptr, TFLOAT, colnum, 1, 1, yerr.front().size(), yerr.front().data(), &status);

        fits_get_colnum(fptr, CASEINSEN, "YERRU", &colnum, &status);
        fits_modify_vector_len(fptr, colnum, yerr.back().size(), &status);
        fits_write_col(fptr, TFLOAT, colnum, 1, 1, yerr.back().size(), yerr.back().data(), &status);
    }

    /* write header */
    char value[FLEN_VALUE] = "Errorbar";
    fits_update_key(fptr, TSTRING, "PLOTTYPE", value, "plot type", &status);
    fits_update_key(fptr, TSTRING, "PLOTNAME", const_cast<char *>(plotname.c_str()), "plot name", &status);
    fits_update_key(fptr, TINT, "AXES", &axesid, "axes id", &status);
    fits_update_key(fptr, TINT, "ID", &id, "plot id", &status);
    fits_update_key(fptr, TSTRING, "LINEWID", const_cast<char *>(linewidth.c_str()), "linewidth", &status);
    fits_update_key(fptr, TSTRING, "COLOR", const_cast<char *>(color.c_str()), "color", &status);
    fits_update_key(fptr, TSTRING, "CAPSIZE", const_cast<char *>(capsize.c_str()), "cap size", &status);
    fits_update_key(fptr, TSTRING, "UPLIMS", const_cast<char *>(uplims.c_str()), "y up limits [true, false]", &status);
    fits_update_key(fptr, TSTRING, "LOLIMS", const_cast<char *>(lolims.c_str()), "y low limits [true, false]", &status);
    fits_update_key(fptr, TSTRING, "XUPLIMS", const_cast<char *>(xuplims.c_str()), "x up limits [true, false]", &status);
    fits_update_key(fptr, TSTRING, "XLOLIMS", const_cast<char *>(xlolims.c_str()), "x low limits [true, false]", &status);

	fits_report_error(stderr, status);

    return *this;    
}
#endif

/*================== PText =================*/
PText & PText::draw(bool logx, bool logy)
{
    cpgsave();

    float left=0., right=1., bottom=0., top=1.;
    cpgqvp(0, &left, &right, &bottom, &top);

    float x1, x2, y1, y2;
    cpgqvsz(1, &x1, &x2, &y1, &y2);
    float width = std::abs(x2-x1);
    float height = std::abs(y2-y1);

    /* set color */
    cpgsci(get_color(color));

    /* set fontsize */
    cpgsch(get_height(fontsize));

    /* set linewidth */
    cpgslw(get_linewidth(linewidth));

    /* set ref position of text*/
    float fjust = 0;
    if (refpos == "left")
        fjust = 0.;
    if (refpos == "center")
        fjust = 0.5;
    else if (refpos == "right")
        fjust = 1.;

    if (xycoords == "data")
    {
        if (logx && !logy)
            cpgptxt(std::log10(x), y, stof(rotation), fjust, text.c_str());
        else if (!logx && logy)
            cpgptxt(x, std::log10(y), stof(rotation), fjust, text.c_str());
        else if (logx && logy)
            cpgptxt(std::log10(x), std::log10(y), stof(rotation), fjust, text.c_str());
        else
            cpgptxt(x, y, stof(rotation), fjust, text.c_str());
    }
    else if (xycoords == "figure fraction")
    {
        float disp = 0.;
        float coord = 0.;
        std::string side = "T";

        if (stof(rotation) == 0.)
        {
            side = "T";
            coord = x;
            if (height < width)
                disp = (y-1.)*40./get_height(fontsize);
            else
                disp = (y-1.)*40./get_height(fontsize)/width*height;
        }
        else if (stof(rotation) == 270. or stof(rotation) == -90.)
        {
            side = "R";
            coord = y;
            if (width < height)
                disp = (x-1.)*40./get_height(fontsize);
            else
                disp = (x-1.)*40./get_height(fontsize)/height*width;
        }

        cpgsvp(0., 1., 0., 1.);
        cpgmtxt(side.c_str(), disp, coord, fjust, text.c_str());
    }
    else
    {
        float disp = 0.;
        float coord = 0.;
        std::string side = "T";

        if (stof(rotation) == 0.)
        {
            side = "T";
            coord = x;
            if (height < width)
                disp = (y-1.)*(top-bottom)*40./get_height(fontsize);
            else
                disp = (y-1.)*(top-bottom)*40./get_height(fontsize)/width*height;
        }
        else if (stof(rotation) == 270. or stof(rotation) == -90.)
        {
            side = "R";
            coord = y;
            if (width < height)
                disp = (x-1.)*(right-left)*40./get_height(fontsize);
            else
                disp = (x-1.)*(right-left)*40./get_height(fontsize)/height*width;
        }

        cpgmtxt(side.c_str(), disp, coord, fjust, text.c_str());
    }

    cpgsvp(left, right, bottom, top);

    cpgunsa();

    return *this;
}

PText * PText::clone()
{
    PText *txt = new PText;

    *txt = *this;

    return txt;
}

#ifdef HAVE_LIBCFITSIO
PText & PText::writehdu(fitsfile *fptr)
{
    int status = 0;
    /* write Meta hdu */
    fits_movnam_hdu(fptr, BINARY_TBL, "META", 0, &status);

    /* write data */
    if (status == BAD_HDU_NUM)
    {
        status = 0;
        char extname[] = "META";
        char *ttype[] = {"AXES", "ID", "X", "Y", "TEXT", "FONTSIZE", "XYCOORDS", "COLOR", "ROTAT", "REFPOS"};
        char *tform[] = {"1J", "1J", "1E", "1E", "128A", "128A", "128A", "128A", "128A", "128A"};
        char *tunit[] = {"", "", "", "", "", "", "", "", "", ""};
        fits_create_tbl(fptr, BINARY_TBL, 0, 10, ttype, tform, tunit, extname, &status);

        char value[FLEN_VALUE] = "Meta";
        fits_update_key(fptr, TSTRING, "PLOTTYPE", value, "plot type", &status);
    }

    long int nrows = 0;
    fits_get_num_rows(fptr, &nrows, &status);

    int colnum = 1;
    fits_get_colnum(fptr, CASEINSEN, "AXES", &colnum, &status);
    fits_write_col(fptr, TINT, colnum, nrows+1, 1, 1, &axesid, &status);

    fits_get_colnum(fptr, CASEINSEN, "ID", &colnum, &status);
    fits_write_col(fptr, TINT, colnum, nrows+1, 1, 1, &id, &status);

    fits_get_colnum(fptr, CASEINSEN, "X", &colnum, &status);
    fits_write_col(fptr, TFLOAT, colnum, nrows+1, 1, 1, &x, &status);

    fits_get_colnum(fptr, CASEINSEN, "Y", &colnum, &status);
    fits_write_col(fptr, TFLOAT, colnum, nrows+1, 1, 1, &y, &status);

    fits_get_colnum(fptr, CASEINSEN, "TEXT", &colnum, &status);
    fits_write_col(fptr, TSTRING, colnum, nrows+1, 1, 1, &text, &status);

    fits_get_colnum(fptr, CASEINSEN, "FONTSIZE", &colnum, &status);
    fits_write_col(fptr, TSTRING, colnum, nrows+1, 1, 1, &fontsize, &status);

    fits_get_colnum(fptr, CASEINSEN, "XYCOORDS", &colnum, &status);
    fits_write_col(fptr, TSTRING, colnum, nrows+1, 1, 1, &xycoords, &status);

    fits_get_colnum(fptr, CASEINSEN, "COLOR", &colnum, &status);
    fits_write_col(fptr, TSTRING, colnum, nrows+1, 1, 1, &color, &status);

    fits_get_colnum(fptr, CASEINSEN, "ROTAT", &colnum, &status);
    fits_write_col(fptr, TSTRING, colnum, nrows+1, 1, 1, &rotation, &status);

    fits_get_colnum(fptr, CASEINSEN, "REFPOS", &colnum, &status);
    fits_write_col(fptr, TSTRING, colnum, nrows+1, 1, 1, &refpos, &status);

	fits_report_error(stderr, status);

    return *this;
}
#endif

/*================== PHistogram =================*/
PHistogram & PHistogram::draw(bool logx, bool logy)
{
    cpgsave();

    /* set linestyle */
    cpgsls(get_linestyle(linestyle));
    /* set linewidth */
    cpgslw(get_linewidth(linewidth));
    /* set color */
    cpgsci(get_color(color));

    int pgflag = 1;
    if (histtype == "bar")
        pgflag = 1;
    else if (histtype == "barfilled")
        pgflag = 3;
    else if (histtype == "step")
        pgflag = 5;

    if (datmin == datmax)
    {
        datmin = *std::min_element(data.begin(), data.end());
        datmax = *std::max_element(data.begin(), data.end());
    }
    
    cpghist(data.size(), data.data(), datmin, datmax, nbin, pgflag);

    cpgunsa();

    return *this;
}

PHistogram * PHistogram::clone()
{
    PHistogram *hist = new PHistogram;

    *hist = *this;

    return hist;
}

void PHistogram::get_win(float &xmin, float &xmax, float &ymin, float &ymax)
{
    xmin = *std::min_element(data.begin(), data.end());
    xmax = *std::max_element(data.begin(), data.end());

    std::vector<int> cnts(nbin, 0);
    for (auto d : data)
    {
        int idx = (d-xmin)/(xmax-xmin)*nbin;
        idx += nbin;
        idx %= nbin;
        cnts[idx]++;
    }

    ymin = 0;
    ymax = *std::max_element(cnts.begin(), cnts.end());
    ymax *= 1.1;
}

#ifdef HAVE_LIBCFITSIO
PHistogram & PHistogram::writehdu(fitsfile *fptr)
{
    int status = 0;
    /* write XYPlot hdu */

    /* write data */
    char extname[] = "HISTOGRAM";
    char *ttype[] = {"DATA"};
    char *tform[] = {"E"};
    char *tunit[] = {""};
    fits_create_tbl(fptr, BINARY_TBL, 1, 1, ttype, tform, tunit, extname, &status);

    int colnum = 1;
    fits_get_colnum(fptr, CASEINSEN, "DATA", &colnum, &status);
    fits_modify_vector_len(fptr, colnum, data.size(), &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, data.size(), data.data(), &status);

    /* write header */
    char value[FLEN_VALUE] = "Histogram";
    fits_update_key(fptr, TSTRING, "PLOTTYPE", value, "plot type", &status);
    fits_update_key(fptr, TSTRING, "PLOTNAME", const_cast<char *>(plotname.c_str()), "plot name", &status);
    fits_update_key(fptr, TINT, "AXES", &axesid, "axes id", &status);
    fits_update_key(fptr, TINT, "ID", &id, "plot id", &status);
    fits_update_key(fptr, TSTRING, "LINESTY", const_cast<char *>(linestyle.c_str()), "line style [-, --, ., -., -...]", &status);
    fits_update_key(fptr, TSTRING, "LINEWID", const_cast<char *>(linewidth.c_str()), "linewidth", &status);
    fits_update_key(fptr, TSTRING, "COLOR", const_cast<char *>(color.c_str()), "color", &status);
    fits_update_key(fptr, TSTRING, "HISTTYPE", const_cast<char *>(histtype.c_str()), "histogram tyle [bar,barfilled,step]", &status);
    fits_update_key(fptr, TFLOAT, "DATMIN", &datmin, "data minimum", &status);
    fits_update_key(fptr, TFLOAT, "DATMAX", &datmax, "data maximum", &status);
    fits_update_key(fptr, TINT, "NBIN", &nbin, "histogram nbin", &status);

	fits_report_error(stderr, status);

    return *this;    
}
#endif

/*================== PStep =================*/
PStep & PStep::draw(bool logx, bool logy)
{
    cpgsave();

    /* set linestyle */
    cpgsls(get_linestyle(linestyle));

    /* set linewidth */
    cpgslw(get_linewidth(linewidth));

    /* set color */
    cpgsci(get_color(color));

    bool center = true;
    if (where != "mid")
    {
        center = false;
    }

    cpgbin(xdata.size(), xdata.data(), ydata.data(), center);

    cpgunsa();

    return *this;
}

PStep * PStep::clone()
{
    PStep *step = new PStep;

    *step = *this;

    return step;
}

void PStep::get_win(float &xmin, float &xmax, float &ymin, float &ymax)
{
    xmin = *(std::min_element(xdata.begin(), xdata.end()));
    xmax = *(std::max_element(xdata.begin(), xdata.end()));
    ymin = *(std::min_element(ydata.begin(), ydata.end()));
    ymax = *(std::max_element(ydata.begin(), ydata.end()));
}

#ifdef HAVE_LIBCFITSIO
PStep & PStep::writehdu(fitsfile *fptr)
{
    int status = 0;
    /* write XYPlot hdu */

    /* write data */
    char extname[] = "STEP";
    char *ttype[] = {"XDATA", "YDATA"};
    char *tform[] = {"E", "E"};
    char *tunit[] = {"", ""};
    fits_create_tbl(fptr, BINARY_TBL, 1, 2, ttype, tform, tunit, extname, &status);

    int colnum = 1;
    fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
    fits_modify_vector_len(fptr, colnum, xdata.size(), &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, xdata.size(), xdata.data(), &status);

    fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
    fits_modify_vector_len(fptr, colnum, ydata.size(), &status);
    fits_write_col(fptr, TFLOAT, colnum, 1, 1, ydata.size(), ydata.data(), &status);

    /* write header */
    char value[FLEN_VALUE] = "Step";
    fits_update_key(fptr, TSTRING, "PLOTTYPE", value, "plot type", &status);
    fits_update_key(fptr, TSTRING, "PLOTNAME", const_cast<char *>(plotname.c_str()), "plot name", &status);
    fits_update_key(fptr, TINT, "AXES", &axesid, "axes id", &status);
    fits_update_key(fptr, TINT, "ID", &id, "plot id", &status);
    fits_update_key(fptr, TSTRING, "LINESTY", const_cast<char *>(linestyle.c_str()), "line style [-, --, ., -., -...]", &status);
    fits_update_key(fptr, TSTRING, "LINEWID", const_cast<char *>(linewidth.c_str()), "linewidth", &status);
    fits_update_key(fptr, TSTRING, "COLOR", const_cast<char *>(color.c_str()), "color", &status);
    fits_update_key(fptr, TSTRING, "WHERE", const_cast<char *>(where.c_str()), "step start position [mid,]", &status);

	fits_report_error(stderr, status);

    return *this;
}
#endif

/*================== Figure =================*/
Figure::Figure()
{
    cnt = 0;

    _width = 0.;
    _aspect = 0.;

    _background_color = "black";
    _default_color = "white";
}

Figure::Figure(float width, float aspect)
{
    cnt = 0;

    _width = width;
    _aspect = aspect;

    _background_color = "black";
    _default_color = "white";
}

Figure::~Figure(){}

Figure & Figure::push(Axes &ax)
{
    ax.id = ++cnt;

    for (auto p=ax.plots.begin(); p!=ax.plots.end(); ++p)
    {
        (*p)->axesid = ax.id;
    }

    ax.get_win();

    axes.push_back(ax);

    return *this;
}

Figure & Figure::show()
{
    if (!cpgopen("/XW")) return *this;

    cpgpap(_width, _aspect);
    
    float r0, g0, b0;
    float r1, g1, b1;

    /* set background color and defalut color*/
    get_rgb(r0, g0, b0, _background_color);
    get_rgb(r1, g1, b1, _default_color);
    cpgscr(0, r0, g0, b0);
    cpgscr(1, r1, g1, b1);

    for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
    {
        ax->draw();
    }
    
    cpgclos();

    return *this;
}

Figure & Figure::save(const std::string &fname)
{
    if (!cpgopen(fname.c_str())) return *this;
    
    cpgpap(_width, _aspect);

    float r0, g0, b0;
    float r1, g1, b1;

    /* set background color and defalut color*/
    get_rgb(r0, g0, b0, _background_color);
    get_rgb(r1, g1, b1, _default_color);
    cpgscr(0, r0, g0, b0);
    cpgscr(1, r1, g1, b1);

    for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
    {
        ax->draw();
    }

    cpgclos();

    return *this;
}

Figure & Figure::savepx(const std::string &fname)
{
#ifdef HAVE_LIBCFITSIO
    int status = 0;

    fitsfile *fptr = NULL;
    fits_create_file(&fptr, fname.c_str(), &status);

    /* write primary hdu*/
    long int naxes[2] = {0, 0};
    fits_create_img(fptr, 32, 2, naxes, &status); 
    fits_update_key(fptr, TSTRING, "FIGNAME", const_cast<char *>(_figname.c_str()), "figure name", &status);
    fits_update_key(fptr, TFLOAT, "WIDTH", &_width, "figure width", &status);
    fits_update_key(fptr, TFLOAT, "ASPECT", &_aspect, "figure aspect ratio height/width", &status);
    fits_update_key(fptr, TSTRING, "BCKCOLOR", const_cast<char *>(_background_color.c_str()), "background color", &status);
    fits_update_key(fptr, TSTRING, "DFTCOLOR", const_cast<char *>(_default_color.c_str()), "default color", &status);
    fits_update_key(fptr, TINT, "CNT", &cnt, "number of axes", &status);
    int score = 0;
    fits_update_key(fptr, TINT, "SCORE", &score, "score", &status);

    /* write axes hdu */
    for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
    {
        ax->writehdu(fptr);
    }
    
    /* write plot hdu */
    for (long int k=0; k<axes.size(); k++)
    {
        for (auto p=axes[k].plots.begin(); p!=axes[k].plots.end(); ++p)
        {
            (*p)->writehdu(fptr);
        }
    }

    fits_close_file(fptr, &status);

    fits_report_error(stderr, status);

    return *this;
#else
    std::cerr<<"Warning: cfitsio not found, can not save to px file"<<std::endl;
    return *this;
#endif
}

Figure & Figure::loadpx(const std::string &fname)
{
#ifdef HAVE_LIBCFITSIO
    int status = 0;

    fitsfile *fptr = NULL;
    fits_open_file(&fptr, fname.c_str(), 0, &status);

    int hdunum = 0;
    fits_get_num_hdus(fptr, &hdunum, &status);

    /* read primary hdu to figure*/
    fits_movabs_hdu(fptr, 1, NULL, &status);

    fits_read_key(fptr, TFLOAT, "WIDTH", &_width, NULL, &status);
    fits_read_key(fptr, TFLOAT, "ASPECT", &_aspect, NULL, &status);
    char value[FLEN_VALUE];
    fits_read_key(fptr, TSTRING, "BCKCOLOR", value, NULL, &status);
    _background_color = value;
    fits_read_key(fptr, TSTRING, "DFTCOLOR", value, NULL, &status);
    _default_color = value;
    fits_read_key(fptr, TINT, "CNT", &cnt, NULL, &status);

    /* read axes hdu to axes */
    fits_movabs_hdu(fptr, 2, NULL, &status);

    long int nrows = 0;
	fits_get_num_rows(fptr, &nrows, &status);

    Axes ax;
    for (long int k=0; k<nrows; k++)
    {
        float array_float[] = {0., 0., 0., 0.};
        int array_int[] = {0, 0, 0, 0};
        char array_logical[] = {0, 0, 0, 0};
        int colnum = 0;

        fits_get_colnum(fptr, CASEINSEN, "ID", &colnum, &status);
        fits_read_col(fptr, TINT, colnum, k+1, 1, 1, NULL, &ax.id, 0, &status);

        fits_get_colnum(fptr, CASEINSEN, "CNT", &colnum, &status);
        fits_read_col(fptr, TINT, colnum, k+1, 1, 1, NULL, &ax.cnt, 0, &status);

        fits_get_colnum(fptr, CASEINSEN, "VIEW", &colnum, &status);
        fits_read_col(fptr, TFLOAT, colnum, k+1, 1, 4, NULL, array_float, 0, &status);
        ax._left = array_float[0];
        ax._right = array_float[1];
        ax._bottom = array_float[2];
        ax._top = array_float[3];

        fits_get_colnum(fptr, CASEINSEN, "WIN", &colnum, &status);
        fits_read_col(fptr, TFLOAT, colnum, k+1, 1, 4, NULL, array_float, 0, &status);
        ax._xmin = array_float[0];
        ax._xmax = array_float[1];
        ax._ymin = array_float[2];
        ax._ymax = array_float[3];

        fits_get_colnum(fptr, CASEINSEN, "FRAME", &colnum, &status);
        fits_read_col(fptr, TLOGICAL, colnum, k+1, 1, 4, NULL, array_logical, 0, &status);
        ax._frameleft = array_logical[0];
        ax._frameright = array_logical[1];
        ax._framebottom = array_logical[2];
        ax._frametop = array_logical[3];

        fits_get_colnum(fptr, CASEINSEN, "LABEL", &colnum, &status);
        fits_read_col(fptr, TLOGICAL, colnum, k+1, 1, 4, NULL, array_logical, 0, &status);
        ax._labelleft = array_logical[0];
        ax._labelright = array_logical[1];
        ax._labelbottom = array_logical[2];
        ax._labeltop = array_logical[3];

        fits_get_colnum(fptr, CASEINSEN, "DIRECT", &colnum, &status);
        fits_read_col(fptr, TLOGICAL, colnum, k+1, 1, 1, NULL, array_logical, 0, &status);
        ax._directionout = array_logical[0];

        fits_get_colnum(fptr, CASEINSEN, "LABELROT", &colnum, &status);
        fits_read_col(fptr, TLOGICAL, colnum, k+1, 1, 1, NULL, array_logical, 0, &status);
        ax._labelrotation = array_logical[0];
    
        fits_get_colnum(fptr, CASEINSEN, "TICKS", &colnum, &status);
        fits_read_col(fptr, TLOGICAL, colnum, k+1, 1, 2, NULL, array_logical, 0, &status);
        ax._minorticks = array_logical[0];
        ax._majorticks = array_logical[1];

        fits_get_colnum(fptr, CASEINSEN, "GRID", &colnum, &status);
        fits_read_col(fptr, TLOGICAL, colnum, k+1, 1, 2, NULL, array_logical, 0, &status);
        ax._gridvertical = array_logical[0];
        ax._gridhorizontal = array_logical[1];

        fits_get_colnum(fptr, CASEINSEN, "LOG", &colnum, &status);
        fits_read_col(fptr, TLOGICAL, colnum, k+1, 1, 2, NULL, array_logical, 0, &status);
        ax._logx = array_logical[0];
        ax._logy = array_logical[1];

        fits_get_colnum(fptr, CASEINSEN, "AXIS", &colnum, &status);
        fits_read_col(fptr, TLOGICAL, colnum, k+1, 1, 1, NULL, array_logical, 0, &status);
        ax._axis = array_logical[0];

        fits_get_colnum(fptr, CASEINSEN, "AUTOSCL", &colnum, &status);
        fits_read_col(fptr, TLOGICAL, colnum, k+1, 1, 2, NULL, array_logical, 0, &status);
        ax._autoscalex = array_logical[0];
        ax._autoscaley = array_logical[1];

        fits_get_colnum(fptr, CASEINSEN, "LIMITS", &colnum, &status);
        fits_read_col(fptr, TLOGICAL, colnum, k+1, 1, 2, NULL, array_logical, 0, &status);
        ax._xlimits = array_logical[0];
        ax._ylimits = array_logical[1];

        int typecode;
	    long int repeat = 0;
	    long int width = 0;
        fits_get_colnum(fptr, CASEINSEN, "XLABEL", &colnum, &status);
	    fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
        char *text = new char [repeat+1];
		fits_read_col(fptr, TSTRING, colnum, k+1, 1, 1, NULL, &text, 0, &status);
        ax._xlabel = text;
	    delete [] text;
        text = NULL;

	    repeat = 0;
	    width = 0;
        fits_get_colnum(fptr, CASEINSEN, "YLABEL", &colnum, &status);
	    fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
        text = new char [repeat+1];
		fits_read_col(fptr, TSTRING, colnum, k+1, 1, 1, NULL, &text, 0, &status);
        ax._ylabel = text;
	    delete [] text;
        text = NULL;

        repeat = 0;
	    width = 0;
        fits_get_colnum(fptr, CASEINSEN, "TITLE", &colnum, &status);
	    fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
        text = new char [repeat+1];
		fits_read_col(fptr, TSTRING, colnum, k+1, 1, 1, NULL, &text, 0, &status);
        ax._title = text;
	    delete [] text;
        text = NULL;

        repeat = 0;
	    width = 0;
        fits_get_colnum(fptr, CASEINSEN, "FSLAB", &colnum, &status);
	    fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
        text = new char [repeat+1];
		fits_read_col(fptr, TSTRING, colnum, k+1, 1, 1, NULL, &text, 0, &status);
        ax._fontsize_label = text;
	    delete [] text;
        text = NULL;

        repeat = 0;
	    width = 0;
        fits_get_colnum(fptr, CASEINSEN, "FSTKLAB", &colnum, &status);
	    fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
        text = new char [repeat+1];
		fits_read_col(fptr, TSTRING, colnum, k+1, 1, 1, NULL, &text, 0, &status);
        ax._fontsize_ticklabel = text;
	    delete [] text;
        text = NULL;

        axes.push_back(ax);
    }

    /* read plot hdu to plots */
    for (long int i=3; i<=hdunum; i++)
    {
        fits_movabs_hdu(fptr, i, NULL, &status);

        fits_read_key(fptr, TSTRING, "PLOTTYPE", value, NULL, &status);
        if (strcmp(value, "Line") == 0)
        {
            std::unique_ptr<PLine> line(new PLine);

            fits_read_key(fptr, TINT, "AXES", &(line->axesid), NULL, &status);
            fits_read_key(fptr, TINT, "ID", &(line->id), NULL, &status);
            fits_read_key(fptr, TSTRING, "LINESTY", value, NULL, &status);
            line->linestyle = value;
            fits_read_key(fptr, TSTRING, "LINEWID", value, NULL, &status);
            line->linewidth = value;
            fits_read_key(fptr, TSTRING, "COLOR", value, NULL, &status);
            line->color = value;

            int colnum = 0;
            int typecode;
	        long int repeat = 0;
	        long int width = 0;
            fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            line->xdata.resize(repeat);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, line->xdata.data(), NULL, &status);

            fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            line->ydata.resize(repeat);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, line->ydata.data(), NULL, &status);

            for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
            {
                if (ax->id == line->axesid)
                {
                    ax->plots.push_back(std::move(line));
                    break;
                }
            }
        }
        else if (strcmp(value, "Axvline") == 0)
        {
            std::unique_ptr<PAxvline> line(new PAxvline);

            fits_read_key(fptr, TINT, "AXES", &(line->axesid), NULL, &status);
            fits_read_key(fptr, TINT, "ID", &(line->id), NULL, &status);
            fits_read_key(fptr, TSTRING, "LINESTY", value, NULL, &status);
            line->linestyle = value;
            fits_read_key(fptr, TSTRING, "LINEWID", value, NULL, &status);
            line->linewidth = value;
            fits_read_key(fptr, TSTRING, "COLOR", value, NULL, &status);
            line->color = value;

            int colnum = 0;
            fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, 1, NULL, &(line->x), NULL, &status);

            fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
            float array_float[2];
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, 2, NULL, array_float, NULL, &status);
            line->_ymin = array_float[0];
            line->_ymax = array_float[1];

            for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
            {
                if (ax->id == line->axesid)
                {
                    ax->plots.push_back(std::move(line));
                    break;
                }
            }
        }
        else if (strcmp(value, "Axhline") == 0)
        {
            std::unique_ptr<PAxhline> line(new PAxhline);

            fits_read_key(fptr, TINT, "AXES", &(line->axesid), NULL, &status);
            fits_read_key(fptr, TINT, "ID", &(line->id), NULL, &status);
            fits_read_key(fptr, TSTRING, "LINESTY", value, NULL, &status);
            line->linestyle = value;
            fits_read_key(fptr, TSTRING, "LINEWID", value, NULL, &status);
            line->linewidth = value;
            fits_read_key(fptr, TSTRING, "COLOR", value, NULL, &status);
            line->color = value;

            int colnum = 0;
            fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
            float array_float[2];
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, 2, NULL, array_float, NULL, &status);
            line->_xmin = array_float[0];
            line->_xmax = array_float[1];

            fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, 1, NULL, &(line->y), NULL, &status);

            for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
            {
                if (ax->id == line->axesid)
                {
                    ax->plots.push_back(std::move(line));
                    break;
                }
            }
        }
        else if (strcmp(value, "Axvline") == 0)
        {
            std::unique_ptr<PAxvline> line(new PAxvline);

            fits_read_key(fptr, TINT, "AXES", &(line->axesid), NULL, &status);
            fits_read_key(fptr, TINT, "ID", &(line->id), NULL, &status);
            fits_read_key(fptr, TSTRING, "LINESTY", value, NULL, &status);
            line->linestyle = value;
            fits_read_key(fptr, TSTRING, "LINEWID", value, NULL, &status);
            line->linewidth = value;
            fits_read_key(fptr, TSTRING, "COLOR", value, NULL, &status);
            line->color = value;

            int colnum = 0;
            fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, 1, NULL, &(line->x), NULL, &status);

            fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
            float array_float[2];
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, 2, NULL, array_float, NULL, &status);
            line->_ymin = array_float[0];
            line->_ymax = array_float[1];

            for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
            {
                if (ax->id == line->axesid)
                {
                    ax->plots.push_back(std::move(line));
                    break;
                }
            }
        }
        else if (strcmp(value, "Axvspan") == 0)
        {
            std::unique_ptr<PAxvspan> span(new PAxvspan);

            fits_read_key(fptr, TINT, "AXES", &(span->axesid), NULL, &status);
            fits_read_key(fptr, TINT, "ID", &(span->id), NULL, &status);
            fits_read_key(fptr, TSTRING, "LINESTY", value, NULL, &status);
            span->linestyle = value;
            fits_read_key(fptr, TSTRING, "LINEWID", value, NULL, &status);
            span->linewidth = value;
            fits_read_key(fptr, TSTRING, "COLOR", value, NULL, &status);
            span->color = value;
            fits_read_key(fptr, TSTRING, "FILLED", value, NULL, &status);
            span->filled = value;

            int colnum = 0;
            fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
            float array_float[2];
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, 2, NULL, array_float, NULL, &status);
            span->_xmin = array_float[0];
            span->_xmax = array_float[1];

            fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, 2, NULL, array_float, NULL, &status);
            span->_ymin = array_float[0];
            span->_ymax = array_float[1];

            for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
            {
                if (ax->id == span->axesid)
                {
                    ax->plots.push_back(std::move(span));
                    break;
                }
            }
        }
        else if (strcmp(value, "Axhspan") == 0)
        {
            std::unique_ptr<PAxhspan> span(new PAxhspan);

            fits_read_key(fptr, TINT, "AXES", &(span->axesid), NULL, &status);
            fits_read_key(fptr, TINT, "ID", &(span->id), NULL, &status);
            fits_read_key(fptr, TSTRING, "LINESTY", value, NULL, &status);
            span->linestyle = value;
            fits_read_key(fptr, TSTRING, "LINEWID", value, NULL, &status);
            span->linewidth = value;
            fits_read_key(fptr, TSTRING, "COLOR", value, NULL, &status);
            span->color = value;
            fits_read_key(fptr, TSTRING, "FILLED", value, NULL, &status);
            span->filled = value;

            int colnum = 0;
            fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
            float array_float[2];
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, 2, NULL, array_float, NULL, &status);
            span->_xmin = array_float[0];
            span->_xmax = array_float[1];

            fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, 2, NULL, array_float, NULL, &status);
            span->_ymin = array_float[0];
            span->_ymax = array_float[1];

            for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
            {
                if (ax->id == span->axesid)
                {
                    ax->plots.push_back(std::move(span));
                    break;
                }
            }
        }
        else if (strcmp(value, "Scatter") == 0)
        {
            std::unique_ptr<PScatter> line(new PScatter);

            fits_read_key(fptr, TINT, "AXES", &(line->axesid), NULL, &status);
            fits_read_key(fptr, TINT, "ID", &(line->id), NULL, &status);
            fits_read_key(fptr, TSTRING, "MARKER", value, NULL, &status);
            line->marker = value;
            fits_read_key(fptr, TSTRING, "MKSIZE", value, NULL, &status);
            line->markersize = value;
            fits_read_key(fptr, TSTRING, "COLOR", value, NULL, &status);
            line->color = value;

            int colnum = 0;
            int typecode;
	        long int repeat = 0;
	        long int width = 0;
            fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            line->xdata.resize(repeat);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, line->xdata.data(), NULL, &status);

            fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            line->ydata.resize(repeat);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, line->ydata.data(), NULL, &status);

            for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
            {
                if (ax->id == line->axesid)
                {
                    ax->plots.push_back(std::move(line));
                    break;
                }
            }
        }
        else if (strcmp(value, "Point") == 0)
        {
            std::unique_ptr<PPoint> line(new PPoint);

            fits_read_key(fptr, TINT, "AXES", &(line->axesid), NULL, &status);
            fits_read_key(fptr, TINT, "ID", &(line->id), NULL, &status);
            fits_read_key(fptr, TSTRING, "MARKER", value, NULL, &status);
            line->marker = value;
            fits_read_key(fptr, TSTRING, "MKSIZE", value, NULL, &status);
            line->markersize = value;
            fits_read_key(fptr, TSTRING, "COLOR", value, NULL, &status);
            line->color = value;

            int colnum = 0;
            fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, 1, NULL, &(line->x), NULL, &status);

            fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, 1, NULL, &(line->y), NULL, &status);

            for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
            {
                if (ax->id == line->axesid)
                {
                    ax->plots.push_back(std::move(line));
                    break;
                }
            }
        }
        else if (strcmp(value, "Errorbar") == 0)
        {
            std::unique_ptr<PErrorbar> line(new PErrorbar);

            fits_read_key(fptr, TINT, "AXES", &(line->axesid), NULL, &status);
            fits_read_key(fptr, TINT, "ID", &(line->id), NULL, &status);
            fits_read_key(fptr, TSTRING, "CAPSIZE", value, NULL, &status);
            line->capsize = value;
            fits_read_key(fptr, TSTRING, "LINEWID", value, NULL, &status);
            line->linewidth = value;
            fits_read_key(fptr, TSTRING, "COLOR", value, NULL, &status);
            line->color = value;
            fits_read_key(fptr, TSTRING, "UPLIMS", value, NULL, &status);
            line->uplims = value;
            fits_read_key(fptr, TSTRING, "LOLIMS", value, NULL, &status);
            line->lolims = value;
            fits_read_key(fptr, TSTRING, "XUPLIMS", value, NULL, &status);
            line->xuplims = value;
            fits_read_key(fptr, TSTRING, "XLOLIMS", value, NULL, &status);
            line->xlolims = value;

            int colnum = 0;
            int typecode;
	        long int repeat = 0;
	        long int width = 0;
            fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            line->xdata.resize(repeat);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, line->xdata.data(), NULL, &status);

            fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            line->ydata.resize(repeat);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, line->ydata.data(), NULL, &status);

            fits_get_colnum(fptr, CASEINSEN, "XERRL", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            if (repeat)
            {
                std::vector<float> xerrl(repeat);
                fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, xerrl.data(), NULL, &status);
                line->xerr.push_back(xerrl);
            }

            fits_get_colnum(fptr, CASEINSEN, "XERRU", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            if (repeat)
            {
                std::vector<float> xerru(repeat);
                fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, xerru.data(), NULL, &status);
                line->xerr.push_back(xerru);
            }

            fits_get_colnum(fptr, CASEINSEN, "YERRL", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            if (repeat)
            {
                std::vector<float> yerrl(repeat);
                fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, yerrl.data(), NULL, &status);
                line->yerr.push_back(yerrl);
            }

            fits_get_colnum(fptr, CASEINSEN, "YERRU", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            if (repeat)
            {
                std::vector<float> yerru(repeat);
                fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, yerru.data(), NULL, &status);
                line->yerr.push_back(yerru);
            }

            for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
            {
                if (ax->id == line->axesid)
                {
                    ax->plots.push_back(std::move(line));
                    break;
                }
            }
        }
        else if (strcmp(value, "Histogram") == 0)
        {
            std::unique_ptr<PHistogram> line(new PHistogram);

            fits_read_key(fptr, TINT, "AXES", &(line->axesid), NULL, &status);
            fits_read_key(fptr, TINT, "ID", &(line->id), NULL, &status);
            fits_read_key(fptr, TSTRING, "HISTTYPE", value, NULL, &status);
            line->histtype = value;
            fits_read_key(fptr, TSTRING, "LINESTY", value, NULL, &status);
            line->linestyle = value;
            fits_read_key(fptr, TSTRING, "LINEWID", value, NULL, &status);
            line->linewidth = value;
            fits_read_key(fptr, TSTRING, "COLOR", value, NULL, &status);
            line->color = value;
            fits_read_key(fptr, TFLOAT, "DATMIN", &(line->datmin), NULL, &status);
            fits_read_key(fptr, TFLOAT, "DATMAX", &(line->datmax), NULL, &status);
            fits_read_key(fptr, TINT, "NBIN", &(line->nbin), NULL, &status);

            int colnum = 0;
            int typecode;
	        long int repeat = 0;
	        long int width = 0;
            fits_get_colnum(fptr, CASEINSEN, "DATA", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            line->data.resize(repeat);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, line->data.data(), NULL, &status);

            for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
            {
                if (ax->id == line->axesid)
                {
                    ax->plots.push_back(std::move(line));
                    break;
                }
            }
        }
        else if (strcmp(value, "Step") == 0)
        {
            std::unique_ptr<PStep> line(new PStep);

            fits_read_key(fptr, TINT, "AXES", &(line->axesid), NULL, &status);
            fits_read_key(fptr, TINT, "ID", &(line->id), NULL, &status);
            fits_read_key(fptr, TSTRING, "LINESTY", value, NULL, &status);
            line->linestyle = value;
            fits_read_key(fptr, TSTRING, "LINEWID", value, NULL, &status);
            line->linewidth = value;
            fits_read_key(fptr, TSTRING, "COLOR", value, NULL, &status);
            line->color = value;
            fits_read_key(fptr, TSTRING, "WHERE", value, NULL, &status);
            line->where = value;

            int colnum = 0;
            int typecode;
	        long int repeat = 0;
	        long int width = 0;
            fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            line->xdata.resize(repeat);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, line->xdata.data(), NULL, &status);

            fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            line->ydata.resize(repeat);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, line->ydata.data(), NULL, &status);

            for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
            {
                if (ax->id == line->axesid)
                {
                    ax->plots.push_back(std::move(line));
                    break;
                }
            }
        }
        else if (strcmp(value, "Pcolor") == 0)
        {
            std::unique_ptr<PPcolor> img(new PPcolor);

            fits_read_key(fptr, TINT, "AXES", &(img->axesid), NULL, &status);
            fits_read_key(fptr, TINT, "ID", &(img->id), NULL, &status);
            fits_read_key(fptr, TSTRING, "CMAP", value, NULL, &status);
            img->cmap = value;

            int colnum = 0;
            int typecode;
	        long int repeat = 0;
	        long int width = 0;
            fits_get_colnum(fptr, CASEINSEN, "XDATA", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            img->xdata.resize(repeat);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, img->xdata.data(), NULL, &status);

            fits_get_colnum(fptr, CASEINSEN, "YDATA", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            img->ydata.resize(repeat);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, img->ydata.data(), NULL, &status);

            fits_get_colnum(fptr, CASEINSEN, "ZDATA", &colnum, &status);
            fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
            img->zdata.resize(repeat);
            fits_read_col(fptr, TFLOAT, colnum, 1, 1, repeat, NULL, img->zdata.data(), NULL, &status);

            for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
            {
                if (ax->id == img->axesid)
                {
                    ax->plots.push_back(std::move(img));
                    break;
                }
            }
        }
        else if (strcmp(value, "Meta") == 0)
        {
            fits_get_num_rows(fptr, &nrows, &status);
            for (long int k=0; k<nrows; k++)
            {
                std::unique_ptr<PText> text(new PText);

                int colnum = 0;
                fits_get_colnum(fptr, CASEINSEN, "AXES", &colnum, &status);
                fits_read_col(fptr, TINT, colnum, k+1, 1, 1, NULL, &(text->axesid), NULL, &status);
            
                fits_get_colnum(fptr, CASEINSEN, "ID", &colnum, &status);
                fits_read_col(fptr, TINT, colnum, k+1, 1, 1, NULL, &(text->id), NULL, &status);

                fits_get_colnum(fptr, CASEINSEN, "X", &colnum, &status);
                fits_read_col(fptr, TFLOAT, colnum, k+1, 1, 1, NULL, &(text->x), NULL, &status);

                fits_get_colnum(fptr, CASEINSEN, "Y", &colnum, &status);
                fits_read_col(fptr, TFLOAT, colnum, k+1, 1, 1, NULL, &(text->y), NULL, &status);


                int typecode;
	            long int repeat = 0;
	            long int width = 0;
                fits_get_colnum(fptr, CASEINSEN, "TEXT", &colnum, &status);
                fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
                char *txt = new char [repeat+1];
                fits_read_col(fptr, TSTRING, colnum, k+1, 1, 1, NULL, &txt, 0, &status);
                text->text = txt;
                delete [] txt;
                txt = NULL;

                repeat = 0;
	            width = 0;
                fits_get_colnum(fptr, CASEINSEN, "XYCOORDS", &colnum, &status);
                fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
                txt = new char [repeat+1];
                fits_read_col(fptr, TSTRING, colnum, k+1, 1, 1, NULL, &txt, 0, &status);
                text->xycoords = txt;
                delete [] txt;
                txt = NULL;

                repeat = 0;
	            width = 0;
                fits_get_colnum(fptr, CASEINSEN, "FONTSIZE", &colnum, &status);
                fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
                txt = new char [repeat+1];
                fits_read_col(fptr, TSTRING, colnum, k+1, 1, 1, NULL, &txt, 0, &status);
                text->fontsize = txt;
                delete [] txt;
                txt = NULL;

                repeat = 0;
	            width = 0;
                fits_get_colnum(fptr, CASEINSEN, "COLOR", &colnum, &status);
                fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
                txt = new char [repeat+1];
                fits_read_col(fptr, TSTRING, colnum, k+1, 1, 1, NULL, &txt, 0, &status);
                text->color = txt;
                delete [] txt;
                txt = NULL;

                repeat = 0;
	            width = 0;
                fits_get_colnum(fptr, CASEINSEN, "ROTAT", &colnum, &status);
                fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
                txt = new char [repeat+1];
                fits_read_col(fptr, TSTRING, colnum, k+1, 1, 1, NULL, &txt, 0, &status);
                text->rotation = txt;
                delete [] txt;
                txt = NULL;

                repeat = 0;
	            width = 0;
                fits_get_colnum(fptr, CASEINSEN, "REFPOS", &colnum, &status);
                fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
                txt = new char [repeat+1];
                fits_read_col(fptr, TSTRING, colnum, k+1, 1, 1, NULL, &txt, 0, &status);
                text->refpos = txt;
                delete [] txt;
                txt = NULL;

                for (auto ax=axes.begin(); ax!=axes.end(); ++ax)
                {
                    if (ax->id == text->axesid)
                    {
                        ax->plots.push_back(std::move(text));
                        break;
                    }
                }
            }
        }
    }

    fits_close_file(fptr, &status);
    fits_report_error(stderr, status);

    return *this;
#else
    std::cerr<<"Warning: cfitsio not found, can not load px file"<<std::endl;
    return *this;
#endif
}

int PlotX::get_linestyle(const std::string &linestyle)
{
    if (linestyle == "") return 1;

    if (linestyle == "-" or linestyle == "solid")
    {
        return 1;
    }
    else if (linestyle == "--" or linestyle == "dashed")
    {
        return 2;
    }
    else if (linestyle == "-." or linestyle == "dashdotted")
    {
        return 3;
    }
    else if (linestyle == "." or linestyle == "dotted")
    {
        return 4;
    }
    else if (linestyle == "-..." or linestyle == "dashdotdotdotted")
    {
        return 5;
    }
    else
    {
        std::cerr<<"Warning: line style not supported"<<std::endl;
        return 1;
    }
}

int PlotX::get_linewidth(const std::string &linewidth)
{
    if (linewidth == "") return 1;

    int lw = 1;
    try
    {
        lw = std::stoi(linewidth);
    }
    catch (const std::exception&)
    {
        std::cerr<<"Warning: linewidth input range is 1-201)"<<std::endl;
        return 1;
    }

    return lw;
}

int PlotX::get_symbol(const std::string &symbol)
{
    if (symbol == "") return 1;

    int s = 1;
    try
    {
        s = std::stoi(symbol);
    }
    catch (const std::exception&)
    {
        std::cerr<<"Warning: marker input range is 0-31)"<<std::endl;
        return 1;
    }

    if (s > 31)
    {
        std::cerr<<"Warning: marker not supported (input range is 0-31)"<<std::endl;
        return 1;
    }

    return s;
}

float PlotX::get_height(const std::string &height)
{
    if (height == "") return 1;

    float h = 1;
    try
    {
        h = std::stof(height);
    }
    catch (const std::exception&)
    {
        std::cerr<<"Warning: input should be a float number)"<<std::endl;
        return 1;
    }

    return h;
}

int PlotX::get_color(const std::string &color)
{
    if (color == "") return 1;

    if (color.rfind("#", 0) == 0)
    {
        unsigned int rgb;
        std::stringstream ss_rgb;
        ss_rgb<<std::hex<<color.substr(color.find("#") + 1);
        ss_rgb>>rgb;

        unsigned int r = (rgb<<8)>>24;
        unsigned int g = (rgb<<16)>>24;
        unsigned int b = (rgb<<24)>>24;

        cpgscr(16, r/255., g/255., b/255.);

        return 16;
    }

    if (color == "k" or color == "black")
    {
        return 0;
    }
    else if (color == "white")
    {
        return 1;
    }
    else if (color == "r" or color == "red")
    {
        return 2;
    }
    else if (color == "g" or color == "green")
    {
        return 3;
    }
    else if (color == "b" or color == "blue")
    {
        return 4;
    }
    else if (color == "c" or color == "cyan")
    {
        return 5;
    }
    else if (color == "m" or color == "magenta")
    {
        return 6;
    }
    else if (color == "y" or color == "yellow")
    {
        return 7;
    }
    else if (color == "orange")
    {
        return 8;
    }
    else if (color == "greenyellow")
    {
        return 9;
    }
    else if (color == "greencyan")
    {
        return 10;
    }
    else if (color == "bluecyan")
    {
        return 11;
    }
    else if (color == "bluemagenta")
    {
        return 12;
    }
    else if (color == "redmagenta")
    {
        return 13;
    }
    else if (color == "darkgray")
    {
        return 14;
    }
    else if (color == "lightgray")
    {
        return 15;
    }
    else
    {
        std::cerr<<"Warning: color not supported"<<std::endl;
        return 1;
    }
}

void PlotX::get_rgb(float &r, float &g, float &b, const std::string &color)
{
   int ci = get_color(color);
   cpgqcr(ci, &r, &g, &b);
}

int PlotX::get_filled(const std::string &filled)
{
    if (filled == "true" or filled == "True" or filled == "on")
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

void PlotX::set_cmap(const std::string &cmap, const std::vector<std::vector<float>> &cm_data)
{
    if (!cm_data.empty())
    {
        float contra = 1.;
        float bright = 0.5;
        const int nc = cm_data[0].size();

        float l[nc];
        for (long int k=0; k<nc; k++)
        {
            l[k] = 0.+k*1./(nc-1);
        }

        cpgctab(l, cm_data[0].data(), cm_data[1].data(), cm_data[2].data(), nc, contra, bright);

        return;
    }

    if (cmap == "" or cmap == "gray")
    {
        float contra = 1.;
        float bright = 0.5;
        const int nc = 2;

        float l[nc] = {0., 1.};
        float r[nc] = {0., 1.};
        float g[nc] = {0., 1.};
        float b[nc] = {0., 1.};

        cpgctab(l, r, g, b, nc, contra, bright);
    }
    else if (cmap == "greys")
    {
        float contra = 1.;
        float bright = 0.5;
        const int nc = 2;

        float l[nc] = {0., 1.};
        float r[nc] = {1., 0.};
        float g[nc] = {1., 0.};
        float b[nc] = {1., 0.};

        cpgctab(l, r, g, b, nc, contra, bright);
    }
    else if (cmap == "viridis")
    {
        float contra = 1.;
        float bright = 0.5;
        const int nc = 12;

        float l[nc];
        for (long int k=0; k<nc; k++)
        {
            l[k] = 0.+k*1./(nc-1);
        }
        float r[nc] = {0.267004, 0.283072, 0.262138, 0.220057, 0.177423, 0.143343, 0.119512, 0.166383, 0.319809, 0.525776, 0.762373, 0.993248};
        float g[nc] = {0.004874, 0.130895, 0.242286, 0.343307, 0.437527, 0.522773, 0.607464, 0.690856, 0.770914, 0.833491, 0.876424, 0.906157};
        float b[nc] = {0.329415, 0.449241, 0.520837, 0.549413, 0.557565, 0.556295, 0.540218, 0.496502, 0.411152, 0.288127, 0.137064, 0.143936};

        cpgctab(l, r, g, b, nc, contra, bright);        
    }
    else if (cmap == "parula")
    {
        float contra = 1.;
        float bright = 0.5;
        const int nc = 64;

        float l[nc];
        for (long int k=0; k<nc; k++)
        {
            l[k] = 0.+k*1./(nc-1);
        }
        float r[nc] = {0.2081    , 0.21162381, 0.21225238, 0.2081    , 0.19590476,
                        0.17072857, 0.12527143, 0.05913333, 0.01169524, 0.00595714,
                        0.01651429, 0.03285238, 0.04981429, 0.06293333, 0.07226667,
                        0.07794286, 0.07934762, 0.07494286, 0.06405714, 0.04877143,
                        0.03434286, 0.0265    , 0.02389048, 0.02309048, 0.02277143,
                        0.0266619 , 0.03837143, 0.05897143, 0.0843    , 0.11329524,
                        0.14527143, 0.18013333, 0.21782857, 0.25864286, 0.30217143,
                        0.34816667, 0.39525714, 0.44200952, 0.48712381, 0.53002857,
                        0.57085714, 0.60985238, 0.6473    , 0.68341905, 0.71840952,
                        0.75248571, 0.78584286, 0.81850476, 0.85065714, 0.88243333,
                        0.91393333, 0.94495714, 0.97389524, 0.99377143, 0.99904286,
                        0.99553333, 0.988     , 0.97885714, 0.9697    , 0.96258571,
                        0.95887143, 0.95982381, 0.9661    , 0.9763};
        float g[nc] = {0.1663    , 0.18978095, 0.21377143, 0.2386    , 0.26445714,
                        0.2919381 , 0.32424286, 0.35983333, 0.38750952, 0.40861429,
                        0.4266    , 0.44304286, 0.45857143, 0.47369048, 0.48866667,
                        0.50398571, 0.52002381, 0.53754286, 0.55698571, 0.57722381,
                        0.59658095, 0.6137    , 0.6286619 , 0.64178571, 0.65348571,
                        0.66419524, 0.67427143, 0.68375714, 0.69283333, 0.7015    ,
                        0.70975714, 0.71765714, 0.72504286, 0.73171429, 0.73760476,
                        0.74243333, 0.7459    , 0.74808095, 0.7490619 , 0.74911429,
                        0.74851905, 0.74731429, 0.7456    , 0.74347619, 0.74113333,
                        0.7384    , 0.73556667, 0.73273333, 0.7299    , 0.72743333,
                        0.72578571, 0.72611429, 0.73139524, 0.74545714, 0.76531429,
                        0.78605714, 0.8066    , 0.82714286, 0.8481381 , 0.87051429,
                        0.8949    , 0.92183333, 0.95144286, 0.9831};
        float b[nc] = {0.5292    , 0.57767619, 0.62697143, 0.67708571, 0.7279    ,
                        0.77924762, 0.83027143, 0.86833333, 0.88195714, 0.88284286,
                        0.87863333, 0.87195714, 0.86405714, 0.8554381 , 0.8467    ,
                        0.83837143, 0.83118095, 0.82627143, 0.82395714, 0.82282857,
                        0.81985238, 0.8135    , 0.8037619 , 0.79126667, 0.77675714,
                        0.76071905, 0.74355238, 0.72538571, 0.70616667, 0.68585714,
                        0.66462857, 0.64243333, 0.6192619 , 0.59542857, 0.57118571,
                        0.54726667, 0.52444286, 0.50331429, 0.48397619, 0.46611429,
                        0.44939048, 0.43368571, 0.4188    , 0.40443333, 0.39047619,
                        0.37681429, 0.36327143, 0.34979048, 0.33602857, 0.3217    ,
                        0.30627619, 0.28864286, 0.26664762, 0.24034762, 0.21641429,
                        0.19665238, 0.17936667, 0.16331429, 0.14745238, 0.1309    ,
                        0.11324286, 0.0948381 , 0.07553333, 0.0538};

        cpgctab(l, r, g, b, nc, contra, bright);   
    }
    else
    {
        std::cerr<<"Warning: colormap not supported"<<std::endl;
    }
}

void PlotX::plot(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::map<std::string, std::string> &options)
{
    Figure fig;
    Axes ax(0.05, 0.98, 0.05, 0.98);
    ax.plot(xdata, ydata, options);
    fig.push(ax);
    fig.show();
}

void PlotX::plot(const std::vector<float> &ydata, const std::map<std::string, std::string> &options)
{
    std::vector<float> xdata(ydata.size());
    std::iota(xdata.begin(), xdata.end(), 0);

    Figure fig;
    Axes ax(0.05, 0.98, 0.05, 0.98);
    ax.plot(xdata, ydata, options);
    fig.push(ax);
    fig.show();
}

void PlotX::pcolor(const std::vector<float> &xdata, const std::vector<float> &ydata, const std::vector<float> &zdata, const std::string &cmap, const std::vector<std::vector<float>> &cm_data, const std::map<std::string, std::string> &options)
{
    Figure fig;
    Axes ax(0.05, 0.98, 0.05, 0.98);
    ax.pcolor(xdata, ydata, zdata, "viridis");
    fig.push(ax);
    fig.show();
}

void PlotX::pcolor(size_t xlen, size_t ylen, const std::vector<float> &zdata, const std::string &cmap, const std::vector<std::vector<float>> &cm_data, const std::map<std::string, std::string> &options)
{
    std::vector<float> xdata(xlen), ydata(ylen);
    std::iota(xdata.begin(), xdata.end(), 0);
    std::iota(ydata.begin(), ydata.end(), 0);

    Figure fig;
    Axes ax(0.05, 0.98, 0.05, 0.98);
    ax.pcolor(xdata, ydata, zdata, "viridis");
    fig.push(ax);
    fig.show();
}
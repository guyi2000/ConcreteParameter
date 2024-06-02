#ifndef UTILITIES_H
#define UTILITIES_H
#include <cmath>
#include "constant.h"

using namespace Concrete_Constant;

class Concrete {
private:
    int __type;
    double __fcu_k;
    double __strain_u;
    double __strain_p;
    double __n;
public:
    Concrete(int type):__type(type) {
        __fcu_k = STRAIN_UTIMATE_K[__type];
        __strain_p = 2.0 + 0.005 * (__fcu_k - 50.0);
        __strain_u = 3.3 - 0.01 * (__fcu_k - 50.0);
        __n = 2.0 - (__fcu_k - 50.0) / 60.0;
        if (__strain_p < 2.0) __strain_p = 2.0;
        if (__strain_u > 3.3) __strain_u = 3.3;
        if (__n > 2.0) __n = 2;
    }
    double get_stress_ratio(double strain) {
        if (strain < 0) return -1;
        if (strain < (__strain_p / __strain_u)) {
            return 1 - pow((1 - (strain * __strain_u) / __strain_p), __n);
        } else if (strain < (1 + 1e-5)) {
            return 1;
        } else {
            return -1;
        }
    }
};

class Circle {
private:
    Concrete* __c;
public:
    Circle(int type) {
        __c = new Concrete(type);
    }
    ~Circle() {
        delete __c;
    }
    double get_width(double h) {
        return 2 * sqrt(0.5 * 0.5 - (h - 0.5) * (h - 0.5));
    }
    double get_area(double h, int num) {
        double delta_x = h / num;
        double total_area = 0;
        for (int i = 0; i < num - 1; i++) {
            total_area += get_delta_area(delta_x * i, delta_x * (i + 1));
        }
        return total_area;
    }
    double get_delta_area(double h1, double h2) {
        return (h2 - h1) * (get_width(h1) + get_width(h2)) / 2;
    }
    double get_total_force(double x_n, int num) {
        double delta_x = x_n / num;
        double total_force = 0;
        for(int i = 0; i < num - 1; i++){
            total_force += __c->get_stress_ratio(1 - (i + 0.5) * delta_x / x_n) *
                    get_delta_area(delta_x * i, delta_x * (i + 1));
        }
        return total_force;
    }
    double get_total_force_x(double x_n, int num) {
        double delta_x = x_n / num;
        double total_force = 0;
        double total_force_moment = 0;
        double delta_area_force;
        for(int i = 0; i < num - 1; i++) {
            delta_area_force = __c->get_stress_ratio(1 - (i + 0.5) * delta_x / x_n) *
                    get_delta_area(delta_x * i, delta_x * (i + 1));
            total_force += delta_area_force;
            total_force_moment +=  delta_area_force * (i + 0.5) * delta_x;
        }
        return total_force_moment / total_force;
    }
    double get_beta(double x_n, int num) {
        double total_force_x = get_total_force_x(x_n, num);
        double p = 1e-5, q = 1.0 - 1e-5;
        double beta = (p + q) / 2;
        double yp, ym;
        while (abs(get_bow_x_specific(beta) - total_force_x) > 1e-5) {
            yp = get_bow_x_specific(p) - total_force_x;
            ym = get_bow_x_specific(beta) - total_force_x;
            if ((yp > 0 && ym > 0) || (yp < 0 && ym < 0)) {
                p = beta;
                beta = (p + q) / 2;
            } else {
                q = beta;
                beta = (p + q) / 2;
            }
        }
        return beta / x_n;
    }
    double get_bow_x_specific(double h) { return get_bow_x(h, 10000); }
    double get_bow_x(double h, int num) {
        double delta_x = h / num;
        double area;
        double total_area = 0;
        double total_area_moment = 0;
        for (int i = 0; i < num - 1; i++) {
            area = get_delta_area(delta_x * i, delta_x * (i + 1));
            total_area += area;
            total_area_moment +=  area * (i + 0.5) * delta_x;
        }
        return total_area_moment / total_area;
    }
    double get_alpha(double x_n, int num) {
        return get_total_force(x_n, num) / get_area(get_beta(x_n, num) * x_n, num);
    }
};

class Ring {
private:
    Concrete* __c;
    double __radius_ratio;
public:
    Ring(int type, double radius_ratio): __radius_ratio(radius_ratio) {
        __c = new Concrete(type);
    }
    ~Ring() {
        delete __c;
    }
    double get_width(double h) {
        if (abs(h - 0.5) - 0.5 * __radius_ratio > 0) {
            return 2 * sqrt(0.5 * 0.5 - (h - 0.5) * (h - 0.5));
        } else {
            return 2 * (sqrt(0.5 * 0.5 - (h - 0.5) * (h - 0.5)) - sqrt(0.5 * 0.5 * __radius_ratio * __radius_ratio - (h - 0.5) * (h - 0.5)));
        }
    }
    double get_area(double h, int num) {
        double delta_x = h / num;
        double total_area = 0;
        for (int i = 0; i < num - 1; i++) {
            total_area += get_delta_area(delta_x * i, delta_x * (i + 1));
        }
        return total_area;
    }
    double get_delta_area(double h1, double h2) {
        return (h2 - h1) * (get_width(h1) + get_width(h2)) / 2;
    }
    double get_total_force(double x_n, int num) {
        double delta_x = x_n / num;
        double total_force = 0;
        for(int i = 0; i < num - 1; i++){
            total_force += __c->get_stress_ratio(1 - (i + 0.5) * delta_x / x_n) *
                    get_delta_area(delta_x * i, delta_x * (i + 1));
        }
        return total_force;
    }
    double get_total_force_x(double x_n, int num) {
        double delta_x = x_n / num;
        double total_force = 0;
        double total_force_moment = 0;
        double delta_area_force;
        for(int i = 0; i < num - 1; i++) {
            delta_area_force = __c->get_stress_ratio(1 - (i + 0.5) * delta_x / x_n) *
                    get_delta_area(delta_x * i, delta_x * (i + 1));
            total_force += delta_area_force;
            total_force_moment +=  delta_area_force * (i + 0.5) * delta_x;
        }
        return total_force_moment / total_force;
    }
    double get_beta(double x_n, int num) {
        double total_force_x = get_total_force_x(x_n, num);
        double p = 1e-5, q = 1.0 - 1e-5;
        double beta = (p + q) / 2;
        double yp, ym;
        while (abs(get_bow_x_specific(beta) - total_force_x) > 1e-5) {
            yp = get_bow_x_specific(p) - total_force_x;
            ym = get_bow_x_specific(beta) - total_force_x;
            if ((yp > 0 && ym > 0) || (yp < 0 && ym < 0)) {
                p = beta;
                beta = (p + q) / 2;
            } else {
                q = beta;
                beta = (p + q) / 2;
            }
        }
        return beta / x_n;
    }
    double get_bow_x_specific(double h) { return get_bow_x(h, 10000); }
    double get_bow_x(double h, int num) {
        double delta_x = h / num;
        double area;
        double total_area = 0;
        double total_area_moment = 0;
        for (int i = 0; i < num - 1; i++) {
            area = get_delta_area(delta_x * i, delta_x * (i + 1));
            total_area += area;
            total_area_moment +=  area * (i + 0.5) * delta_x;
        }
        return total_area_moment / total_area;
    }
    double get_alpha(double x_n, int num) {
        return get_total_force(x_n, num) / get_area(get_beta(x_n, num) * x_n, num);
    }
};

class TSection {
private:
    Concrete* __c;
    double __h_ratio;
    double __b_ratio;
public:
    TSection(int type, double h_ratio, double b_ratio): __h_ratio(h_ratio), __b_ratio(b_ratio) {
        __c = new Concrete(type);
    }
    ~TSection() {
        delete __c;
    }
    double get_width(double h) {
        if (h < __h_ratio) {
            return 1;
        } else {
            return __b_ratio;
        }
    }
    double get_area(double h, int num) {
        double delta_x = h / num;
        double total_area = 0;
        for (int i = 0; i < num - 1; i++) {
            total_area += get_delta_area(delta_x * i, delta_x * (i + 1));
        }
        return total_area;
    }
    double get_delta_area(double h1, double h2) {
        return (h2 - h1) * (get_width(h1) + get_width(h2)) / 2;
    }
    double get_total_force(double x_n, int num) {
        double delta_x = x_n / num;
        double total_force = 0;
        for(int i = 0; i < num - 1; i++){
            total_force += __c->get_stress_ratio(1 - (i + 0.5) * delta_x / x_n) *
                    get_delta_area(delta_x * i, delta_x * (i + 1));
        }
        return total_force;
    }
    double get_total_force_x(double x_n, int num) {
        double delta_x = x_n / num;
        double total_force = 0;
        double total_force_moment = 0;
        double delta_area_force;
        for(int i = 0; i < num - 1; i++) {
            delta_area_force = __c->get_stress_ratio(1 - (i + 0.5) * delta_x / x_n) *
                    get_delta_area(delta_x * i, delta_x * (i + 1));
            total_force += delta_area_force;
            total_force_moment +=  delta_area_force * (i + 0.5) * delta_x;
        }
        return total_force_moment / total_force;
    }
    double get_beta(double x_n, int num) {
        double total_force_x = get_total_force_x(x_n, num);
        double p = 1e-5, q = 1.0 - 1e-5;
        double beta = (p + q) / 2;
        double yp, ym;
        while (abs(get_bow_x_specific(beta) - total_force_x) > 1e-5) {
            yp = get_bow_x_specific(p) - total_force_x;
            ym = get_bow_x_specific(beta) - total_force_x;
            if ((yp > 0 && ym > 0) || (yp < 0 && ym < 0)) {
                p = beta;
                beta = (p + q) / 2;
            } else {
                q = beta;
                beta = (p + q) / 2;
            }
        }
        return beta / x_n;
    }
    double get_bow_x_specific(double h) { return get_bow_x(h, 10000); }
    double get_bow_x(double h, int num) {
        double delta_x = h / num;
        double area;
        double total_area = 0;
        double total_area_moment = 0;
        for (int i = 0; i < num - 1; i++) {
            area = get_delta_area(delta_x * i, delta_x * (i + 1));
            total_area += area;
            total_area_moment +=  area * (i + 0.5) * delta_x;
        }
        return total_area_moment / total_area;
    }
    double get_alpha(double x_n, int num) {
        return get_total_force(x_n, num) / get_area(get_beta(x_n, num) * x_n, num);
    }
};

#endif // UTILITIES_H

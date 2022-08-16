/*
 * main.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opengl/opengl.hpp"
using namespace atto;

/**
 * Set data and test prefix directory names
 */
static const std::string WritePrefix("/tmp/");
static const std::string ReadPrefix("../images/");
static const std::vector<std::string> ImageData {
    "baboon_512.png",
    "color-wheel-80x80-blue.png",
    "color-wheel-80x80-bluea.png",
    "color-wheel-80x80-red.png",
    "color-wheel-80x80-reda.png",
    "color-wheel-80x80-rgb.png",
    "color-wheel-80x80-rgba.png",
    "color-wheel-rgb.png",
    "color-wheel-rgba.png",
    "fruits_512.png",
    "monarch_512.png",
    "pool_512.png",
    "tulips_512.png",
    "watch_512.png"};

/**
 * main test client
 */
int main(int argc, char const *argv[])
{
    /* ---- Test red images ---------------------------------------------------
     */
#if 1
    gl::Image img1(
        ReadPrefix + std::string("color-wheel-80x80-red.png"));
    img1.infolog("\nimg1: color-wheel-80x80-rgb");
    img1.write_png(
        WritePrefix + std::string("out.img1_color-wheel-red.png"));
    img1.write_ppma(
        WritePrefix + std::string("out.img1_color-wheel-red_p3.ppm"));
    img1.write_ppmb(
        WritePrefix + std::string("out.img1_color-wheel-red_p6.ppm"));

    gl::Image img2(
        ReadPrefix + std::string("color-wheel-80x80-reda.png"));
    img2.infolog("\nimg2: color-wheel-80x80-rgba");
    img2.write_png(
        WritePrefix + std::string("out.img2_color-wheel-reda.png"));
    img2.write_ppma(
        WritePrefix + std::string("out.img2_color-wheel-reda_p3.ppm"));
    img2.write_ppmb(
        WritePrefix + std::string("out.img2_color-wheel-reda_p6.ppm"));
#endif


    /* ---- Test blue images --------------------------------------------------
     */
#if 1
    gl::Image img3(
        ReadPrefix + std::string("color-wheel-80x80-blue.png"));
    img3.infolog("\nimg3: color-wheel-80x80-rgb");
    img3.write_png(
        WritePrefix + std::string("out.img3_color-wheel-blue.png"));
    img3.write_ppma(
        WritePrefix + std::string("out.img3_color-wheel-blue_p3.ppm"));
    img3.write_ppmb(
        WritePrefix + std::string("out.img3_color-wheel-blue_p6.ppm"));

    gl::Image img4(
        ReadPrefix + std::string("color-wheel-80x80-bluea.png"));
    img4.infolog("\nimg4: color-wheel-80x80-rgba");
    img4.write_png(
        WritePrefix + std::string("out.img4_color-wheel-bluea.png"));
    img4.write_ppma(
        WritePrefix + std::string("out.img4_color-wheel-bluea_p3.ppm"));
    img4.write_ppmb(
        WritePrefix + std::string("out.img4_color-wheel-bluea_p6.ppm"));
#endif


    /* ---- Test image load ---------------------------------------------------
     *
     * Create an image with a given a (600x600x32bpp)
     */
#if 1
    std::printf("\n--------\n");
    gl::Image img5(600, 600, 32);
    img5.infolog("\nimg5: 600x600x32");

    std::printf("\nimg5.load(std::string(color-wheel-80x80-rgb.png)\n");
    img5.load(ReadPrefix + std::string("color-wheel-80x80-rgb.png"));
    img5.infolog("img5:");

    std::printf("\nimg5.load(std::string(color-wheel-80x80-rgba.png)\n");
    img5.load(ReadPrefix + std::string("color-wheel-80x80-rgba.png"));
    img5.infolog("img5:");


    /* ---- Test image move ---------------------------------------------------
     *
     * Create an image from a rgba file
     */
    std::printf("\n--------\n");
    gl::Image img6(
        ReadPrefix + std::string("color-wheel-80x80-rgb.png"));
    img6.infolog("\nimg6: color-wheel-rgba");
    img6.write_ppma(
        WritePrefix + std::string("out.img6_color-wheel-80x80-rgb_p3.ppm"));
    img6.write_ppmb(
        WritePrefix + std::string("out.img6_color-wheel-80x80-rgb_p6.ppm"));

    /*
     * Test image move/copy assignment
     */
    std::printf("\nimg6 = std::move(img5)\n");
    img6 = img5;
    img5.infolog("img5:");
    img6.infolog("img6:");

    std::printf("\nimg6 = img5\n");
    img6 = std::move(img5);
    img5.infolog("img5:");
    img6.infolog("img6:");

    /* Test copy constructor */
    std::printf("\nimg7(img6):\n");
    gl::Image img7(img6);
    img6.infolog("img6:");
    img7.infolog("img7:");
    img7.write_ppma(
        WritePrefix + std::string("out.img7_color-wheel-80x80-rgb_p3.ppm"));
    img7.write_ppmb(
        WritePrefix + std::string("out.img7_color-wheel-80x80-rgb_p6.ppm"));

    /* Test move constructor */
    std::printf("\nimg8(std::move(img6)):\n");
    gl::Image img8(std::move(img6));
    img6.infolog("img6:");
    img8.infolog("img8:");
    img8.write_ppma(
        WritePrefix + std::string("out.img8_color-wheel-80x80-rgb_p3.ppm"));
    img8.write_ppmb(
        WritePrefix + std::string("out.img8_color-wheel-80x80-rgb_p6.ppm"));


    /* ---- Test FreeImage save -----------------------------------------------
     *
     * Create an image from a rgb file
     */
    std::printf("\n--------\n");
    std::printf("\nimg9(std::string(color-wheel-80x80-rgb.png)\n");
    gl::Image img9(
        ReadPrefix + std::string("color-wheel-80x80-rgb.png"));
    img9.infolog("img9:");
    img9.write_ppma(
        WritePrefix + std::string("out.img9_color-wheel-80x80-rgb_p3.ppm"));
    img9.write_ppmb(
        WritePrefix + std::string("out.img9_color-wheel-80x80-rgb_p6.ppm"));

    /*
     * Test Load again.
     */
    std::printf("img9.load(std::string(color-wheel-80x80-rgba.png)\n");
    img9.load(
        ReadPrefix + std::string("color-wheel-80x80-rgba.png"));
    img9.infolog("img9:");
    std::printf("img9.load(std::string(color-wheel-80x80-rgba.png)\n");
    img9.load(
        ReadPrefix + std::string("color-wheel-80x80-rgba.png"));
    img9.infolog("img9:");
    img9.write_ppma(
        WritePrefix + std::string("out.img9_color-wheel-80x80-rgba_p3.ppm"));
    img9.write_ppmb(
        WritePrefix + std::string("out.img9_color-wheel-80x80-rgba_p6.ppm"));


    /* ---- Test image clone --------------------------------------------------
     *
     * Clone an image from a rgb file
     */
    std::printf("\n--------\n");
    std::printf("\nimg10(img8.clone())\n");
    gl::Image img10(img8);
    img8.infolog("img8:");
    img10.infolog("img10:");

    std::printf("img10.swap(0,1), swap red/green, rgba -> grba\n");
    img10.swap(0,1);          /* swap red/green, rgba -> grba */
    std::printf("img10.swap(0,2,false), swap green/blue grba -> brga\n");
    img10.swap(0,2, false);  /* swap green/blue grba -> brga */

    img10.write_ppma(
        WritePrefix + std::string("out.img10_color-wheel-80x80-rgba_p3.ppm"));
    img10.write_ppmb(
        WritePrefix + std::string("out.img10_color-wheel-80x80-rgba_p6.ppm"));
#endif
}

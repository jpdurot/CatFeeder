#!/bin/bash
mkdir -p apk
jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore keystore.jks platforms/android/app/build/outputs/apk/release/app-release-unsigned.apk CatFeeder
cp platforms/android/app/build/outputs/apk/release/app-release-unsigned.apk apk/CatFeeder.apk
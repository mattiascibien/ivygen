#!/bin/bash
set -ev
sudo add-apt-repository --yes ppa:ubuntu-sdk-team/ppa
sudo add-apt-repository --yes ppa:george-edison55/precise-backports
sudo apt-get update -qq

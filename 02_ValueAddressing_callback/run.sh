#!/bin/bash
stdbuf -oL ./build/a.out |& tee run.log
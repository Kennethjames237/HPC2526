#!/bin/sh

# Da eseguire sul PC se dotato di scheda NVIDIA

hostname
nvidia-smi -L
./deviceQuery


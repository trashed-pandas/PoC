#!/bin/sh
xxd -p payload.template >payload.hex
xxd -r -p payload.hex >payload

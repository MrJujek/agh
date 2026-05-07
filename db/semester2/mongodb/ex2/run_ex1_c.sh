#!/usr/bin/env bash
set -euo pipefail
mongosh --file ex2/compare_utils.mongodb.js --file ex2/ex1_c.mongodb.js

# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

import subprocess, os

os.makedirs('../../build/docs', exist_ok=True)
os.makedirs('_static', exist_ok=True)
os.makedirs('_templates', exist_ok=True)
subprocess.call('doxygen', shell=True)

# -- Project information -----------------------------------------------------

project = 'WP43S'
copyright = '2021 Over_score'
author = 'Over_score'

# -- General configuration ---------------------------------------------------

extensions = [
  "breathe"
]

breathe_projects = { "WP43S": "../../build/docs/xml" }
breathe_default_project = "WP43S"
breathe_domain_by_extension = {"h": "c" , "c": "c"}

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

html_title = 'WP43S'
html_theme = 'furo'
html_static_path = ['_static']

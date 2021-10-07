# tikz2svg

Tikz2svg is a simple Perl script that converts a TikZ file written in LaTeX to SVG using pdflatex and pdf2svg. It requires the standalone and TikZ packages, and runs on UNIX-like systems because of stdout handling using /dev/null and /dev/stdout.

## Usage

```bash
# help
tikz2svg -h
tikz2svg --help

# file to file
tikz2svg input.tex output.svg

# file to stdout
tikz2svg input.tex
tikz2svg input.tex > output.svg
tikz2svg input.tex -			# equivalent to tikz2svg input.tex
tikz2svg input.tex - > output.svg	# equivalent to tikz2svg input.tex > output.svg

# stdin to file
tikz2svg - output.svg			# prompt
tikz2svg - output.svg < input.tex

# stdin to stdout
tikz2svg				# prompt
tikz2svg > output.svg			# prompt
tikz2svg - output.svg			# prompt
tikz2svg < input.tex > output.svg
tikz2svg < input.tex
tikz2svg - < input.tex			# equivalent to tikz2svg < input.tex
tikz2svg - - < input.tex		# equivalent to tikz2svg < input.tex
```

## Input formats

tikz2svg tries to handle the following input formats: `\documentclass{`non-`standalone}`, `\documentclass{standalone}`, `\begin{document}`, `\begin{tikzpicture}` or `\tikz`, and just TikZ commands.

### Non-standalone document class

```latex
\documentclass{article} % will be converted to \documentclass[tikz]{standalone}

\begin{document}
\begin{tikzpicture}[scale=2,font=\Large]
\coordinate (energy tip) at (0,0.1);
\coordinate (voltage waveform tip) at (0,-4.1);
\draw[fill=red!20] (energy tip)
  --++(2,1)--++(-1,0)--++(0,1)--++(-2,0)--++(0,-1)--++(-1,0)--++(2,-1);
\draw (energy tip)++(0,1) node {Energy};
\draw[fill=blue!20] (-2,-2) rectangle (2,0);
\draw (0,-1) node {Sensor};
\draw[fill=green!20] (voltage waveform tip)
  --++(2,1)--++(-1,0)--++(0,1)--++(-2,0)--++(0,-1)--++(-1,0)--++(2,-1);
\draw (voltage waveform tip)++(0,1) node[text width=2cm,align=center] {Voltage\\waveform};
\draw[domain=-3.141:3.141,smooth] plot (\x,{-5.5+sin(10*\x r)*cos(7*\x r)});
\end{tikzpicture}
\end{document}
```
### Standalone document class

```latex
% The entire source will be compiled as is
\documentclass{standalone}
\begin{document}
\begin{tikzpicture}[scale=2,font=\Large]
\coordinate (energy tip) at (0,0.1);
\coordinate (voltage waveform tip) at (0,-4.1);
\draw[fill=red!20] (energy tip)
  --++(2,1)--++(-1,0)--++(0,1)--++(-2,0)--++(0,-1)--++(-1,0)--++(2,-1);
\draw (energy tip)++(0,1) node {Energy};
\draw[fill=blue!20] (-2,-2) rectangle (2,0);
\draw (0,-1) node {Sensor};
\draw[fill=green!20] (voltage waveform tip)
  --++(2,1)--++(-1,0)--++(0,1)--++(-2,0)--++(0,-1)--++(-1,0)--++(2,-1);
\draw (voltage waveform tip)++(0,1) node[text width=2cm,align=center] {Voltage\\waveform};
\draw[domain=-3.141:3.141,smooth] plot (\x,{-5.5+sin(10*\x r)*cos(7*\x r)});
\end{tikzpicture}
\end{document}
```

### Document

```latex
% \documentclass[tikz]{standalone} will be added

\begin{document}
\begin{tikzpicture}[scale=2,font=\Large]
\coordinate (energy tip) at (0,0.1);
\coordinate (voltage waveform tip) at (0,-4.1);
\draw[fill=red!20] (energy tip)
  --++(2,1)--++(-1,0)--++(0,1)--++(-2,0)--++(0,-1)--++(-1,0)--++(2,-1);
\draw (energy tip)++(0,1) node {Energy};
\draw[fill=blue!20] (-2,-2) rectangle (2,0);
\draw (0,-1) node {Sensor};
\draw[fill=green!20] (voltage waveform tip)
  --++(2,1)--++(-1,0)--++(0,1)--++(-2,0)--++(0,-1)--++(-1,0)--++(2,-1);
\draw (voltage waveform tip)++(0,1) node[text width=2cm,align=center] {Voltage\\waveform};
\draw[domain=-3.141:3.141,smooth] plot (\x,{-5.5+sin(10*\x r)*cos(7*\x r)});
\end{tikzpicture}
\end{document}
```

### TikZ picture

```latex
% The following two lines will be added
% \documentclass[tikz]{standalone}
% \begin{document}

\begin{tikzpicture}[scale=2,font=\Large]
\coordinate (energy tip) at (0,0.1);
\coordinate (voltage waveform tip) at (0,-4.1);
\draw[fill=red!20] (energy tip)
  --++(2,1)--++(-1,0)--++(0,1)--++(-2,0)--++(0,-1)--++(-1,0)--++(2,-1);
\draw (energy tip)++(0,1) node {Energy};
\draw[fill=blue!20] (-2,-2) rectangle (2,0);
\draw (0,-1) node {Sensor};
\draw[fill=green!20] (voltage waveform tip)
  --++(2,1)--++(-1,0)--++(0,1)--++(-2,0)--++(0,-1)--++(-1,0)--++(2,-1);
\draw (voltage waveform tip)++(0,1) node[text width=2cm,align=center] {Voltage\\waveform};
\draw[domain=-3.141:3.141,smooth] plot (\x,{-5.5+sin(10*\x r)*cos(7*\x r)});
\end{tikzpicture}

% \end{document} will be added
```

### TikZ commands

```latex
% The following three lines will be added
% \documentclass[tikz]{standalone}
% \begin{document}
% \begin{tikzpicture}

\coordinate (energy tip) at (0,0.1);
\coordinate (voltage waveform tip) at (0,-4.1);
\draw[fill=red!20] (energy tip)
  --++(2,1)--++(-1,0)--++(0,1)--++(-2,0)--++(0,-1)--++(-1,0)--++(2,-1);
\draw (energy tip)++(0,1) node {Energy};
\draw[fill=blue!20] (-2,-2) rectangle (2,0);
\draw (0,-1) node {Sensor};
\draw[fill=green!20] (voltage waveform tip)
  --++(2,1)--++(-1,0)--++(0,1)--++(-2,0)--++(0,-1)--++(-1,0)--++(2,-1);
\draw (voltage waveform tip)++(0,1) node[text width=2cm,align=center] {Voltage\\waveform};
\draw[domain=-3.141:3.141,smooth] plot (\x,{-5.5+sin(10*\x r)*cos(7*\x r)});

% The following two lines will be added
% \end{tikzpicture}
% \end{document}
```

## Server/client scripts

Since most web-hosting providers do not support LaTeX, the standalone tikz2svg script won't run on their server. These server/client scripts can be installed to support TikZ to SVG conversion on a web-hosting server without a LaTeX installation.

### tikz2svg.cgi

This CGI script can be installed on a private web server with pdflatex and pdf2svg, and communicate with the tikz2svgc client to provide TikZ to SVG conversion services.

### tikz2svgc

This script is the client part of tikz2svg.cgi.

### .tikz2svgrc

This file is a configuration file for both tikz2svg.cgi and tikz2svgc.

```perl
# tikz2svg.cgi settings
pdflatex => "/path/to/pdflatex",
pdf2svg => "/path/to/pdf2svg",

# tikz2svgc setting
server => "https://example.com/tikz2svg.cgi",

# tikz2svg.cgi and tikz2svgc setting
secret => "tikz2svg.cgi and tikz2svgc share this secret for communication. Change this to your choice.",
```

### .htaccess

It is very important to secure .tikz2svgrc if it is not outside a web-serviceable directory.

```apache
RewriteEngine On
RewriteBase /

RewriteCond %{HTTPS} off
RewriteRule ^(.*)$ https://%{HTTP_HOST}%{REQUEST_URI} [R,L]

# Allow your IP addresses only
Order Allow,Deny
Allow from 127.0.0.1

# Secure your .tikz2svgrc
<Files ~ "^\.">
	Deny from all
</Files>
```

## License

Copyright (C) 2019, Huidae Cho <<https://idea.isnew.info>>  
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

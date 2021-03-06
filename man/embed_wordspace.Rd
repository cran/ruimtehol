% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/r-all-the-things.R
\name{embed_wordspace}
\alias{embed_wordspace}
\title{Build a Starspace model which calculates word embeddings}
\usage{
embed_wordspace(
  x,
  model = "wordspace.bin",
  early_stopping = 0.75,
  useBytes = FALSE,
  ...
)
}
\arguments{
\item{x}{a character vector of text where tokens are separated by spaces}

\item{model}{name of the model which will be saved, passed on to \code{\link{starspace}}}

\item{early_stopping}{the percentage of the data that will be used as training data. If set to a value smaller than 1, 1-\code{early_stopping} percentage of the data which will be used as the validation set and early stopping will be executed. Defaults to 0.75.}

\item{useBytes}{set to TRUE to avoid re-encoding when writing out train and/or test files. See \code{\link[base]{writeLines}} for details}

\item{...}{further arguments passed on to \code{\link{starspace}} except file, trainMode and fileFormat}
}
\value{
an object of class \code{textspace} as returned by \code{\link{starspace}}.
}
\description{
Build a Starspace model which calculates word embeddings
}
\examples{
\dontshow{if(require(udpipe))\{}
library(udpipe)
data(brussels_reviews, package = "udpipe")
x <- subset(brussels_reviews, language == "nl")
x <- strsplit(x$feedback, "\\\\W")
x <- lapply(x, FUN = function(x) x[x != ""])
x <- sapply(x, FUN = function(x) paste(x, collapse = " "))
x <- tolower(x)

set.seed(123456789)
model <- embed_wordspace(x, early_stopping = 0.9,
                         dim = 15, ws = 7, epoch = 10, minCount = 5, ngrams = 1,
                         maxTrainTime = 2) ## maxTrainTime only set for CRAN
plot(model)
wordvectors <- as.matrix(model)

mostsimilar <- embedding_similarity(wordvectors, wordvectors["weekend", ])
head(sort(mostsimilar[, 1], decreasing = TRUE), 10)
mostsimilar <- embedding_similarity(wordvectors, wordvectors["vriendelijk", ])
head(sort(mostsimilar[, 1], decreasing = TRUE), 10)
mostsimilar <- embedding_similarity(wordvectors, wordvectors["grote", ])
head(sort(mostsimilar[, 1], decreasing = TRUE), 10)
\dontshow{\} # End of main if statement running only if the required packages are installed}
}

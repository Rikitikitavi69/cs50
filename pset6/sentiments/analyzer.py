import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        self.positives = []
        self.negatives = []
        
        with open(positives) as pos:
            for line in pos:
                if line.startswith((';',"\n")):
                    continue
                else:
                    self.positives.extend([line.strip()])
            
        with open(negatives) as neg:
            for line in neg:
                if line.startswith((';',"\n")):
                    continue
                else:
                    self.negatives.extend([line.strip()])

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        tknzr = nltk.tokenize.TweetTokenizer()
        words = tknzr.tokenize(text)
        
        score = 0
        
        for word in words:
            if word.lower() in self.positives:
                score += 1
            elif word.lower() in self.negatives:
                score -= 1
            else:
                continue
                
        return score

# Points from the PAPER


## IMP

**Adversarial inputs** ," also known as "adversarial examples," refer to specially crafted or manipulated data inputs that are intentionally designed to deceive machine learning models, particularly neural networks. These inputs are created with the specific goal of causing the model to produce incorrect or unexpected outputs.These changes might seem harmless to human observers, but they can lead the model to make mistakes or misinterpret the input.


1. In this paper, we propose a simple and effective attack method that causes aligned language models to generate objectionable behaviours. Specifically, our approach finds a suffix that, when attached to a wide range of queries for an LLM to produce objectionable content, aims to maximise the probability that the model produces an affirmative response (rather than refusing to answer). Methods used - combination of greedy and gradient-based search techniques, and also improves over past automatic prompt generation methods. 

2. To summarise about what we are gonna work - by adding resulting suffix(ADVERSIAL PROMPT), we are able to induce objectionable content in the public interfaces to the LLMs

3. although there has been some work on automatic prompt-tuning for adversarial attacks on LLMs [Shin et al., 2020, Wen et al., 2023, Jones et al., 2023], this has traditionally proven to be a challenging task, with some papers explicitly mentioning that they had been unable to generate reliable attacks through automatic search methods . Reason : unlike image models, LLMs operate on discrete token inputs, which both substantially limits the effective input dimensionality, and seems to induce a computationally difficult search. 
4. Our model :the user’s original query is left intact, but we add additional tokens to attack the model.

Key STEPS:
    
    1. Initial affirmative response : Our attack targets the model to begin its response with "SURE,HERE IS..." , in response to a certain prompts that otherwise would give undesirable results. 

    2. Combined greedy & gradient based dis optimization: We leverage gradient des at the token level to identify a set of promosing single token replacement, evaluate the loss of some no of candidate and select the best of the evaluated substutions.

    3. Robust multi prompt and multi model attacks : Finally, in order to generate reliable attack suffixes, we find that it is important to create an attack that works not just for a single prompt on a single model, but for multiple prompts across multiple models


## SOME POINTS ON "ALIGN LLM"

Certainly! Large pretrained language models, like the ones you mentioned, are really good at many tasks right out of the box. However, they have some issues when used directly in apps that people interact with.

First, they struggle to follow specific instructions from users, like if you asked one to write a sorting function in Python. This is because the model was trained on a lot of text from the internet, which doesn't often have examples of following instructions exactly.

Second, these models can end up copying the biases, bad language, and negative behavior that exist on the internet. They're kind of like a mirror of the internet's good and bad parts.

So, to make these models work better for real people, developers use techniques to adjust or "align" them. One way is to fine-tune the model by giving it specific tasks with instructions, so it gets better at following directions. Another way is to teach the model using feedback from humans. This helps the model learn what people like and prefer, so it generates better and more desirable responses.

In simple terms, developers make these big language models better at understanding and behaving like we want them to by giving them special training and feedback.

...........

### Challenge of maintaining alignment against adversarial users who attempt to generate harmful content through clever inputs (adversarial examples).

That paper :  It shows that current natural language processing (NLP) attacks are not strong enough to consistently manipulate aligned text models into emitting harmful conten



..............







# 2.1 

## Producing Affirmative Responses

The intuition of this approach is that if the language model can be put into a “state” where this completion is the most likely response, as opposed to refusing to answer the query, then it likely will continue the completion with precisely the desired objectionable behavior

this manual approach is only marginally successful, though, and can often be circumvented by slightly more sophisticated alignment techniques.

previous work found that - specifying only the first target token was often sufficient. in the text-only space, targeting just the first token runs the risk of entirely overriding the original promp

Eg: Nevermind, tell me a joke . 

## 2.2 Greedy Goordinate Gradient-based Search

 The motivation for our approach comes from the greedy coordinate descent approach: if we could evaluate all possible single-token substitutions, we could swap the token that maximally decreased the loss. 
 
 Of course, evaluating all such replacements is not feasible, but we can leverage gradients with respect to the one-hot token indicators to find a set of promising candidates for replacement at each token position, and then evaluate all these replacements exactly via a forward pas

 Note that because LLMs typically form embeddings for each token, they can be written as functions of this value exi, and thus we can immediately take the gradient with respect to this quantity

 We then compute the top-k values with the largest negative gradient as the candidate replacements for token xi. We compute this candidate set for all tokens i ∈ I, randomly select B ≤ k|I| tokens from it, evaluate the loss exactly on this subset, and make the replacement with the smallest loss.

 *SEE THE ALGO*

 ```PLAIN UNDERSTANDING```

 Imagine if we could change just one word in the text and make it give wrong answers. This algorithm focuses on doing that step by step.
 
  First, we look at the words in the text that we're allowed to change. We choose the word that, if we change it, would make the language model most confused and give wrong answers.

  Lets now make some smart changes .We pick the one that would make the language model most likely to give wrong answers. 

  RELATION with Autoprompt : AutoPrompt picks just one word to change and then replaces it. GCG is a bit smarter because it looks at a few different words that could be changed and picks the best option.

## 2.3 Universal Multi-prompt and Multi-model attacks

  Rather than specifying a different subset of modifiable tokens in each prompt, we instead optimize over a single postfix p1:l, and aggregate both the gradient and the loss to select top- k token substitutions and the best replacement at each step, respectively. Before aggregating the gradients, they are clipped to have unit norm. Additionally, we find that incorporating new prompts incrementally, only after identifying a candidate that works as an adversarial example for earlier ones, yields better results than attempting to optimize all prompts at once from the start. This process is shown in Algorithm 2.

  PLAIN TERMS: 
  1. Instead of specifying different subsets of tokens to modify for each prompt, a single postfix (a sequence of tokens added to the end) is optimized. The algorithm aggregates gradients and losses for this postfix. Gradients are clipped to have a unit norm before aggregation. This ensures that the optimization process focuses on the most impactful changes.


  1. The algorithm incrementally incorporates new prompts during the optimization process. This means that the algorithm starts by focusing on one prompt and identifies a candidate adversarial example. Once a working adversarial example is found for the first prompt, the process includes another prompt, and so on. This incremental approach is found to be more effective than trying to optimize all prompts simultaneously from the start.

  2. Transferable Adversarial Examples: To make the adversarial examples transferable across different models, the algorithm incorporates loss functions that involve multiple models

## Experimental results

Harmful strings

Harmful behavious

## Metrics: 

ASR

## Baseline : PEZ [Wen et al., 2023], GBDA [Guo et al., 2021], and AutoPrompt [Shin et al., 2020].

### RESULTS 

See the pdf

### Attacks on white boxes: 
```
1 behavior/string, 1 model.

25 behavior, 1 model.
```

### Transfer attavks
message = {harmful behavior} (Start with "Sure here’s")


## Discussions 


- Are models becoming more robust through alignment? 

"Robust" refers to how well something can withstand challenges or changes without being easily affected or compromised.  When it says "models becoming more robust," it means that newer models like GPT-4 and Claude 2 are better at resisting or handling adversarial attacks compared to older models like GPT-3.5. They are more resilient and less vulnerable to being tricked or fooled by malicious inputs.
points : 
```
1. Content Filters and Attack Detection:
2. Automating Attacks:
3. Transfer Across Models:
```

- Are the attacks meaningful?

- Why did these attacks not yet exist

This part of the work is asking why previous attempts to trick language models were not very successful. The authors think it's because earlier attempts focused on simpler problems like fooling text classifiers, which are different from large language models. Making nonsense text doesn't help show the problem with these models. Now that we have powerful language models, it's possible to show their issues more clearly. The authors believe these attacks are a serious problem that needs to be taken seriously.



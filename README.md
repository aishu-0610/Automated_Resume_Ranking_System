# Automated_Resume_Ranking_System

Overview
This project implements a deterministic, rule-based ranking system for plain-text resumes, written in C++. The objective is not automated hiring, but to explore algorithmic modelling, feature separation, and heuristic scoring in a transparent and analyzable way.
The system scores resumes using weighted keyword and skill matches, combined with a simple numerical extraction of years of experience, and returns the top three ranked candidates.

Motivation and Academic Context
My academic interests lie in algorithmic problem solving, graph theory, and combinatorial optimisation, with particular emphasis on understanding the trade-offs between heuristic methods and solutions with provable guarantees 

This project serves as a small, self-contained exercise in:
- designing a heuristic scoring function,
- separating categorical and numerical features, and
- reasoning about how simple rules affect ranking outcomes.
The emphasis is on clarity and correctness of the algorithmic design, rather than model complexity.

Methodology
Each resume is scored using three independent components:
- Keyword score: presence-based matching with fixed weights
- Skill score: presence-based matching handled separately from keywords
- Experience score: numerical extraction of years of experience using simple token-based parsing
Experience is treated as a quantitative feature, while keywords and skills are treated as categorical features, reflecting their fundamentally different roles in scoring.

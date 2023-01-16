# CSS432_23W_Roshambo
Project Description
Overall Requirement

    Your term project is very open-ended. You will apply your knowledge from this class to decide how to implement the project exactly.
    Some key facts about this project
        You will be implementing a multiplayer, network-based game. The game must be text-based. It can be real-time or turn-based. It can be a well-known game (like poker) or a game completely of your own creation. I recommend getting the protocol's basics working and then building a fun game on top of it since you will be graded primarily on the protocol.
        You will design the game networking protocol yourself. I will give you some protocol requirements, but anything beyond those is up to you. The communication function between players must be implemented using sockets for network communication (no higher-level networking constructs unless you build them yourself).
        If your implementation must use some existing libraries, you must check with the instructor before using the libraries. In general, if the libraries do not provide high-level networking functions, they are allowed to use them, but you shall check with the instructor for assurance. In particular, if you decide to use Unity for game development, you must confirm the networking library with the instructor.
        Deliverables will include the game, documentation of how to play the game, and how the networking protocol works.
        You may work individually or with one teammate. If you work with a teammate, every member needs to document how he/she distributed the work, coordinated your development, and handled any problems that arose. In general, I will not handle any disputes between work partners or give extensions if one member of your group decides not to do any work, so choose your team wisely.
        Each team will have up to 10 minutes to demonstrate their game in class and give the other students a chance to play the game or see the game played briefly. The students will then evaluate the game. If you need to set up your server, please prepare your laptop ahead of time.
        There is no specified programming language. You can use C++, Java, Python, etc.
        You must use software version control. Use Bitbucket or Github with your .edu address and mark the repository as private. Please name your repository as CSS432_23W_TeamName. You must give read access to the instructor - using the instructor's UW email. Your repository must be private to others.

Detailed Requirement

    It must be a game! If it is not a game, you get 0 for this project!
    (5 points) Your game protocol will be written at the Application layer.
    (5 points) Your game may be in any network architecture model, including client/server, P2P, or hybrid. It must be written using sockets for network communication (no high-level networking constructs unless you build them yourself).
    (5 points) Your game should not crash at any time! Any crash will cause you 5 points.
    (50 points) Your game must define and implement the following high-level (application layer) protocol. You can add additional features to this if you want (depending on your game, you might have to). This is a very common set of functions required by network games. If you decide to implement a very innovative game that inherently cannot support these functions, talk to me ASAP, and we can discuss an alternative protocol.
        (5 points) Register. Registration announces that you are available to play the game and includes any contact information necessary for a player to establish a game with another player.  This could optionally include a username/password to log into the system, but if you include this information, do not send the password in plaintext.
        (5 points) List Games. This provides a list of possible games that the user can join.  The return value should include any necessary information you might want the user to know about the games (for example, which level the game is playing or who is in the game).
        (5 points) Create Game. This allows the user to announce that he/she has started a game, and other people are allowed to join.  This message should include any information you want the user to know about the game, such as what level the game is playing or who is in the game.
        (5 points) Join Game. This allows the user to join a game.  The message should include any information necessary for the user to participate in the game.
        (5 points) Exit Game. This allows the user to exit the game gracefully.
        (5 points) Unregister. This should remove the user from the list of possible players if you're maintaining one.
        (20 points) Application-Specific protocol. You will have to design your own protocol for transmitting data between the players in the game (for example, showing your opponent that your tank has moved to a different position in a battle game or showing your opponent that you raised the bet in poker)
    Documentation - 10 points
    Bonus Features (5 points)
        Implement a global scoreboard. This would keep a record of who has the most points or wins (or whatever your metric for scoring is).  If you implement your game as a P2P game, you should use your bootstrap server as a scoreboard server (a distributed scoreboard would be great, but it is beyond the scope of the course, so unless you're motivated to do so, you can take the more centralized and simpler approach). The scoreboard is a global scoreboard for all games and all registered players, but not for an individual game.
    The best game - extra 5 points
        The team with the highest evaluation will get five extra points
        The second place will get three extra points
        The third place will get one extra point.
        If there is a tie, the instructor's evaluation will be used to break the tie.

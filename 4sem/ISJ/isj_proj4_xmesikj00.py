#!/usr/bin/env python3

def first_pair_given_sum(nums: list[int], target: int) -> tuple[int,int]|None:
    """Returns the tuple of two first numbers from the input list that sum up
       to the target, or None if no such numbers are in the list.

    >>> first_pair_given_sum([1,2,1,5,3,6], 12) 

    >>> first_pair_given_sum([1,2,5,3,6], 8)
    (5, 3)

    >>> first_pair_given_sum([6], 6)

    >>> first_pair_given_sum([1,2,2,5,3,6], 4)
    (2, 2)
    """
         
    for i in range(len(nums)-1):
        if nums[i]+nums[i+1] == target:
            return nums[i], nums[i+1]

    

def to_be_credited_alpha(lead_actors: list[str], 
                         actors_in_scenes: list[list[str]]
                        ) -> list[str]:
    """Returns the list of cast members that need to be credited at the end
       (are not among lead actors listed first) in alphabetical order.

    >>> to_be_credited_alpha(['Olivier', 'Caine', 'Channing'], [(1, ['Caine', 'Matthews']), (2, ['Olivier', 'Matthews', 'Martin']), (3, ['Morris', 'Caine', 'Cawthorne'])]) 
    ['Cawthorne', 'Martin', 'Matthews', 'Morris']
    """
    duplicates = set()
    return sorted([actor for scene,cast in actors_in_scenes for actor in cast if actor not in lead_actors and actor not in duplicates and not duplicates.add(actor)])




def order_of_appearance(actors_in_scenes: list[tuple[int, list[str]]]
                       ) -> list[str]:
    """Returns the list of cast members in order of appearance 
       (listed the first time they appear in a scene in the input list).

    >>> order_of_appearance([(1, ['Caine', 'Matthews']), (2, ['Olivier', 'Matthews', 'Martin']), (3, ['Morris', 'Caine', 'Cawthorne'])]) 
    ['Caine', 'Matthews', 'Olivier', 'Martin', 'Morris', 'Cawthorne']
    """
    
    actors_in_scenes_flat_list = [actor for (scene, cast) in actors_in_scenes for actor in cast]
    duplicates = set()
    return [actor for actor in actors_in_scenes_flat_list if actor not in duplicates and not duplicates.add(actor)]


if __name__ == "__main__":
    import doctest
    doctest.testmod()

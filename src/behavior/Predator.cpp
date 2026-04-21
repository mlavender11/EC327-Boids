// #include "Predator.h"

// Predator::Predator(): Boids() 
// {
//     target_flock = nullptr;
//     target = nullptr;
// }

// Predator::Predator(Flock& new_target_flock): Boids() 
// {
//     target_flock = &new_target_flock;
//     target = nullptr;
// }

// void Predator::get_target(){
//     if(target_flock == nullptr || target_flock->GetSizeOfFlock() == 0) return;

//     target = &target_flock->Get_Friendly(0);

//     if(target_flock->GetSizeOfFlock() == 1)return;


//     for(int i = 1; i < target_flock->GetSizeOfFlock(); i++){
//         if (getDistance(*this, target_flock->Get_Friendly(i)) < getDistance(*this, *target)){
//             target = &target_flock->Get_Friendly(i);
//             }
//     }
// }
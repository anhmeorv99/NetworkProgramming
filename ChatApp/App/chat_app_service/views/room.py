from django_filters import rest_framework as filters
from rest_framework import viewsets, status
from rest_framework.permissions import AllowAny
from rest_framework import filters as rest_filters
from rest_framework.response import Response

from chat_app_service.serializer.room import RoomSerializer
from chat_app_service.models import Room
from collections import OrderedDict


class RoomViewSet(viewsets.ModelViewSet):
    queryset = Room.objects.all()
    permission_classes = (AllowAny,)
    serializer_class = RoomSerializer
    filter_backends = (filters.DjangoFilterBackend, rest_filters.SearchFilter, rest_filters.OrderingFilter)

    def list(self, request, *args, **kwargs):
        queryset = self.filter_queryset(self.get_queryset())
        member = request.query_params.get('member')

        serializer = self.get_serializer(queryset, many=True)
        if member is not None:

            member = member.split(',')
            for room in serializer.data:
                flag = True
                room_dict = dict(OrderedDict(room))
                for id_member in room_dict['member']:
                    if len(room_dict['member']) != len(member):
                        flag = False
                        break
                    if str(id_member) not in member:
                        flag = False
                if flag:
                    return Response([room])
            if flag == False:
                return Response(status=status.HTTP_404_NOT_FOUND)
        else:
            return Response(serializer.data)
